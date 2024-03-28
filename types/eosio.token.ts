// Generated by eosio-abi2ts 1.2.2 - eosio::abi/1.2

export type Asset = string
export type Name = string
export type Symbol = string

export interface Account {
  balance: Asset
}

export interface Close {
  owner: Name
  symbol: Symbol
}

export interface Create {
  issuer: Name
  maximum_supply: Asset
}

export interface CurrencyStats {
  supply: Asset
  max_supply: Asset
  issuer: Name
}

export interface Issue {
  to: Name
  quantity: Asset
  memo: string
}

export interface Open {
  owner: Name
  symbol: Symbol
  ram_payer: Name
}

export interface Retire {
  quantity: Asset
  memo: string
}

export interface Transfer {
  from: Name
  to: Name
  quantity: Asset
  memo: string
}

