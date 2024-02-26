using namespace eosio;


/**
\ingroup public_actions
\brief Авторизация принятого решения советом
*
* Этот метод позволяет председателю совета утвердить принятое решение совета. 
*
* @param coopname Имя кооператива
* @param chairman Имя председателя совета кооператива
* @param decision_id Идентификатор решения для авторизации
* 
* @note Авторизация требуется от аккаунта: @p chairman
*/
void soviet::authorize(eosio::name coopname, eosio::name chairman, uint64_t decision_id, document document) { 
  require_auth(chairman);

  boards_index boards(_soviet, coopname.value);
  autosigner_index autosigner(_soviet, coopname.value);
  
  decisions_index decisions(_soviet, coopname.value);
  auto decision = decisions.find(decision_id);
  eosio::check(decision != decisions.end(), "Документ не найден");
  auto board = get_board_by_type_or_fail(coopname, "soviet"_n);
  eosio::check(board.is_valid_chairman(chairman), "Только председатель совета может утвердить решение");
  
  eosio::check(decision -> approved == true, "Консенсус совета по решению не достигнут");

  struct document clean;

  decisions.modify(decision, chairman, [&](auto &d){
    d.authorized = !decision -> authorized;
    d.authorization = document;
    // if (!decision -> authorized == true)
    //   d.authorization = document;
    // else d.authorization = clean;
  });

  auto signer = autosigner.find(decision -> id);
  
  if (signer != autosigner.end())
    autosigner.erase(signer);

}

/**
\ingroup public_actions
\brief Создание нового совета кооператива
*
* Этот метод позволяет председателю кооператива создать новый совет с указанными членами и параметрами. Если совет создается как совет кооператива, председатель должен быть указан в списке членов совета.
*
* @param coopname Имя кооператива
* @param chairman Имя председателя кооператива
* @param type Тип совета. Доступные типы:
*   - **soviet** - Совет кооператива (Board of Members)
*   - **executive** - Правление (Executive Board)
*   - **audit** - Ревизионный комитет (Audit and Revision Board)
*   - **other** - Другая комиссия (Other committee)
* @param members Список членов совета
* @param name Название совета
* @param description Описание совета
* 
* @note Авторизация требуется от аккаунта: @p chairman
*/
void soviet::createboard(eosio::name coopname, eosio::name chairman, eosio::name type, std::vector<board_member> members, std::string name, std::string description){

  eosio::check(has_auth(chairman) || has_auth(coopname), "Недостаточно прав доступа");
  eosio::name payer = has_auth(chairman) ? chairman : coopname;

  orgs_index orgs(_registrator, _registrator.value);
  auto org = orgs.find(coopname.value);
  eosio::check(org != orgs.end(), "Организация не найдена");
  eosio::check(org -> is_coop(), "Организация - не кооператив");
  
  if (type == "soviet"_n) {
    bool is_exist = check_for_exist_board_by_type(coopname, "soviet"_n);
    eosio::check(is_exist == false, "Совет кооператива уже создан");
    
    bool has_chairman = false;
    std::set<eosio::name> usernames;

    // Проверка на наличие председателя в списке членов совета
    for (const auto& m : members) {
        eosio::check(usernames.insert(m.username).second, "Обнаружено повторение username");

        if (m.position == "chairman"_n) {
            eosio::check(m.username == chairman, "Только председатель может создать совет кооператива");
            has_chairman = true;
        };
    };

    eosio::check(has_chairman, "Председатель кооператива должен быть указан в членах совета");

    //Добавляем председателя в пайщики кооператива автоматически
    participants_index participants(_soviet, coopname.value);
    auto cooperative = get_cooperative_or_fail(coopname);
    participants.emplace(_soviet, [&](auto &m){
      m.username = chairman;
      m.created_at = eosio::time_point_sec(eosio::current_time_point().sec_since_epoch());
      m.last_update = eosio::time_point_sec(eosio::current_time_point().sec_since_epoch());
      m.last_min_pay = eosio::time_point_sec(eosio::current_time_point().sec_since_epoch());
      m.status = "accepted"_n;
      m.is_initial = true;
      m.is_minimum = true;
      m.has_vote = true;    

      m.wallet.available = asset(0, cooperative.initial.symbol);
      m.wallet.blocked = asset(0, cooperative.initial.symbol);
      m.wallet.minimum = cooperative.minimum; 
    });


    action(
      permission_level{ _soviet, "active"_n},
      _fund,
      "init"_n,
      std::make_tuple(coopname, cooperative.initial)
    ).send();

  } else {
    
    auto soviet = get_board_by_type_or_fail(coopname, "soviet"_n);
    eosio::check(soviet.is_valid_chairman(chairman), "Только председатель кооператива может создать совет");
  }

  boards_index boards(_soviet, coopname.value);

  boards.emplace(payer, [&](auto &b) {
    b.id = boards.available_primary_key();
    b.type = type;
    b.members = members;
    b.name = name;
    b.description = description;
    b.created_at = eosio::time_point_sec(eosio::current_time_point().sec_since_epoch());
    b.last_update = eosio::time_point_sec(eosio::current_time_point().sec_since_epoch());
  });

}




/**
\ingroup public_actions
\brief Обновление совета или комиссии
*
* Этот метод позволяет председателю кооператива обновить совет с указанными членами. 
*
* @param coopname Имя кооператива
* @param chairman Имя председателя кооператива
* @param type Тип совета. Доступные типы:
*   - **soviet** - Совет кооператива (Board of Members)
*   - **executive** - Правление (Executive Board)
*   - **audit** - Ревизионный комитет (Audit and Revision Board)
*   - **other** - Другая комиссия (Other committee)
* @param members Список членов совета
* @param name Название совета
* @param description Описание совета
* 
* @note Авторизация требуется от аккаунта: @p chairman
*/
void soviet::updateboard(eosio::name coopname, eosio::name chairman, uint64_t board_id, std::vector<board_member> members, std::string name, std::string description){

  require_auth(chairman);
  orgs_index orgs(_registrator, _registrator.value);
  auto org = orgs.find(coopname.value);
  eosio::check(org != orgs.end(), "Организация не найдена");
  eosio::check(org -> is_coop(), "Организация - не кооператив");

  boards_index boards(_soviet, coopname.value);
  auto board = boards.find(board_id);
  eosio::check(board != boards.end(), "Доска не найдена");

  eosio::check(board -> is_valid_chairman(chairman), "Только председатель кооператива может обновить доску");

  if (board -> type == "soviet"_n) {
    bool has_chairman = false;
    std::set<eosio::name> usernames;


    for (const auto& m : members) {
        eosio::check(usernames.insert(m.username).second, "Обнаружено повторение username");

        if (m.position == "chairman"_n) {
            eosio::check(m.username == chairman, "Только председатель может создать совет кооператива");
            has_chairman = true;
        };
    };
    eosio::check(has_chairman, "Председатель кооператива должен быть указан в членах совета");
  };

  
  boards.modify(board, chairman, [&](auto &b) {
    b.members = members;
    b.name = name;
    b.description = description;
    b.last_update = eosio::time_point_sec(eosio::current_time_point().sec_since_epoch());
  });

}
