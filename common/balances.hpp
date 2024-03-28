
/**
 * @ingroup public_tables
 * @brief Таблица `deposits` отслеживает депозиты в контракте GATEWAY.
 */

struct [[eosio::table, eosio::contract(GATEWAY)]] deposits {
    uint64_t id; /*!< Уникальный идентификатор записи депозита */
    eosio::name username; /*!< Имя аккаунта пользователя, совершившего депозит */
    eosio::name coopname; /*!< Имя аккаунта кооператива, в контексте которого совершается депозит */
    eosio::name type; /*!< Тип взноса (deposit | initial) */
    
    eosio::name token_contract; /*!< Имя аккаунта контракта токена для депозита */
    eosio::asset quantity; /*!< Количество средств во внутренней валюте */

    eosio::name status; /*!< Статус депозита */
    std::string link; /*!< Ссылка на дополнительную информацию или внешние данные */
    std::string memo; /*!< Примечание к депозиту */
    eosio::time_point_sec expired_at; ///< Время истечения срока давности

    uint64_t primary_key() const { return id; } /*!< Возвращает id как первичный ключ */
    uint64_t by_username() const { return username.value; } /*!< Индекс по имени пользователя */
    uint64_t by_status() const { return status.value; } /*!< Индекс по статусу депозита */
    uint64_t by_expired() const { return expired_at.sec_since_epoch(); } /*!< Индекс по статусу истечения */
};

typedef eosio::multi_index<
    "deposits"_n, deposits,
    eosio::indexed_by<"byusername"_n, eosio::const_mem_fun<deposits, uint64_t, &deposits::by_username>>,
    eosio::indexed_by<"bystatus"_n, eosio::const_mem_fun<deposits, uint64_t, &deposits::by_status>>,
    eosio::indexed_by<"byexpired"_n, eosio::const_mem_fun<deposits, uint64_t, &deposits::by_expired>>
> deposits_index; /*!< Мультииндекс для доступа и манипуляции данными таблицы `deposits` */



/**
 * @ingroup public_tables
 * @brief Таблица `withdraws` отслеживает операции вывода средств в контракте GATEWAY.
 */
struct [[eosio::table, eosio::contract(GATEWAY)]] withdraws {
    uint64_t id; /*!< Уникальный идентификатор записи вывода */
    eosio::name username; /*!< Имя пользователя, осуществляющего вывод средств */
    eosio::name coopname; /*!< Имя аккаунта кооператива, в рамках которого осуществляется вывод */
    std::string bank_data_id; /*!< Строковый идентификатор банковских реквизитов в приватной базе данных кооператива */
    eosio::name token_contract; /*!< Имя контракта контракта токена для вывода */
    eosio::asset quantity; /*!< Количество средств для вывода во внутренней валюте */
    eosio::name status; /*!< Статус операции вывода */
    document document; /*!< Заявление на возврат */
    std::string memo; /*!< Примечание к операции вывода */
    eosio::time_point_sec created_at; ///< Время истечения срока давности

    uint64_t primary_key() const { return id; } /*!< Возвращает id как первичный ключ */
    uint64_t by_coopname() const { return coopname.value; } /*!< Индекс по имени кооператива */
    uint64_t by_username() const { return username.value; } /*!< Индекс по имени пользователя */
    uint64_t by_status() const { return status.value; } /*!< Индекс по статусу операции вывода */
    uint64_t by_created() const { return created_at.sec_since_epoch(); } /*!< Индекс по статусу истечения */
};

  typedef eosio::multi_index<
    "withdraws"_n, withdraws,
    eosio::indexed_by<"byusername"_n, eosio::const_mem_fun<withdraws, uint64_t, &withdraws::by_username>>,
    eosio::indexed_by<"bycoopname"_n, eosio::const_mem_fun<withdraws, uint64_t, &withdraws::by_coopname>>,
    eosio::indexed_by<"bystatus"_n, eosio::const_mem_fun<withdraws, uint64_t, &withdraws::by_status>>,
    eosio::indexed_by<"bycreated"_n, eosio::const_mem_fun<withdraws, uint64_t, &withdraws::by_created>>
    > withdraws_index; /*!< Мультииндекс для доступа и манипуляции данными таблицы `withdraws` */



/**
\ingroup public_tables
\brief Структура таблицы баланса.

@details Структура, представляющая записи баланса в контрактах. Используется при отправке токенов в контракты _marketplace или _registrator.

@note Таблица хранится в области памяти пользователя (_registrator | _marketplace, username)
*/
struct balances_base {
  uint64_t id;          /*!< идентификатор баланса */
  eosio::name contract; /*!< имя контракта токена */
  eosio::asset quantity; /*!< количество токенов на балансе */

  uint64_t primary_key() const { return id; } /*!< return id - primary_key */

  uint128_t byconsym() const {
    return combine_ids(contract.value, quantity.symbol.code().raw());
  } /*!< возвращает уникальный индекс, сформированный из значения contract и символа токена */
};

typedef eosio::multi_index<"balances"_n, balances_base, eosio::indexed_by<"byconsym"_n, eosio::const_mem_fun<balances_base, uint128_t, &balances_base::byconsym>>> balances_index; /*!< Тип мультииндекса для таблицы балансов */


struct [[eosio::table, eosio::contract(SOVIET)]] wallets {
  eosio::name username;
  eosio::name coopname;
  eosio::asset available;
  eosio::asset blocked;
  eosio::asset minimum;

  uint64_t primary_key() const { return username.value; } /*!< return username - primary_key */
  
};

typedef eosio::multi_index<"wallets"_n, wallets> wallets_index;


struct [[eosio::table, eosio::contract(SOVIET)]] progwallets {
  uint64_t id;
  eosio::name coopname;
  uint64_t program_id;
  eosio::name username;
  eosio::asset available;
  
  uint64_t primary_key() const { return id; } /*!< return id - primary_key */
  uint64_t by_username() const { return username.value; } /*!< username - secondary_key */
  uint64_t by_program() const { return program_id; } /*!< username - secondary_key */

  uint128_t by_username_and_program() const {
    return combine_ids(username.value, program_id);
  } /*!< возвращает уникальный индекс, сформированный из значения username и program_id */
};

typedef eosio::multi_index<"progwallets"_n, progwallets, 
  eosio::indexed_by<"byusername"_n, eosio::const_mem_fun<progwallets, uint64_t, &progwallets::by_username>>,
  eosio::indexed_by<"byprogram"_n, eosio::const_mem_fun<progwallets, uint64_t, &progwallets::by_program>>,
  eosio::indexed_by<"byuserprog"_n, eosio::const_mem_fun<progwallets, uint128_t, &progwallets::by_username_and_program>>
> progwallets_index; /*!< Тип мультииндекса для таблицы кошелька программ */



void add_balance(eosio::name source, eosio::name username, eosio::asset quantity,
                                  eosio::name contract) {
  // Если баланс не найден, создаем новую запись.
  // В противном случае, увеличиваем существующий баланс.

  
  eosio::check(username != ""_n, "В поле memo должен быть указан получатель баланса");
  balances_index balances(source, username.value);

  auto balances_by_contract_and_symbol =
      balances.template get_index<"byconsym"_n>();
  auto contract_and_symbol_index =
      combine_ids(contract.value, quantity.symbol.code().raw());

  auto balance =
      balances_by_contract_and_symbol.find(contract_and_symbol_index);

  if (balance == balances_by_contract_and_symbol.end()) {
    balances.emplace(source, [&](auto &b) {
      b.id = balances.available_primary_key();
      b.contract = contract;
      b.quantity = quantity;
    });
  } else {
    balances_by_contract_and_symbol.modify(
        balance, source, [&](auto &b) { b.quantity += quantity; });
  };
}


void sub_balance(eosio::name source, eosio::name username, eosio::asset quantity,
                              eosio::name contract) {
  // Если после вычитания баланс равен нулю, удаляем запись.
  // В противном случае, уменьшаем существующий баланс.
  
  balances_index balances(source, username.value);

  auto balances_by_contract_and_symbol =
      balances.template get_index<"byconsym"_n>();
  auto contract_and_symbol_index =
      combine_ids(contract.value, quantity.symbol.code().raw());

  auto balance =
      balances_by_contract_and_symbol.find(contract_and_symbol_index);

  eosio::check(balance != balances_by_contract_and_symbol.end(),
               "Баланс не найден");

  eosio::check(balance->quantity >= quantity, "Недостаточный баланс");

  if (balance->quantity == quantity) {

    balances_by_contract_and_symbol.erase(balance);

  } else {

    balances_by_contract_and_symbol.modify(
        balance, source, [&](auto &b) { b.quantity -= quantity; });
  }
}

