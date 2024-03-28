// Generated by eosio-abi2ts 1.2.2 - eosio::abi/1.2

export type Asset = string
export type Name = string
export type TimePointSec = string
export type Uint64 = number | string

export interface Accfunds {
  id: Uint64
  coopname: Name
  contract: Name
  name: string
  description: string
  percent: Uint64
  available: Asset
  withdrawed: Asset
}

export interface Addaccum {
  coopname: Name
  fund_id: Uint64
  quantity: Asset
}

export interface Addcirculate {
  coopname: Name
  quantity: Asset
}

export interface Addexpense {
  coopname: Name
  fund_id: Uint64
  quantity: Asset
}

export interface Authorize {
  coopname: Name
  type: Name
  withdraw_id: Uint64
}

export interface Complete {
  coopname: Name
  username: Name
  withdraw_id: Uint64
}

export interface Counts extends CountsBase {
}

export interface CountsBase {
  key: Name
  secondary_key: Name
  value: Uint64
}

export interface Createfund {
  coopname: Name
  username: Name
  type: Name
  contract: Name
  name: string
  description: string
  percent: Uint64
}

export interface Delfund {
  coopname: Name
  username: Name
  type: Name
  fund_id: Uint64
}

export interface Document {
  hash: string
  pkey: string
  sign: string
  meta: string
}

export interface Editfund {
  coopname: Name
  username: Name
  type: Name
  fund_id: Uint64
  contract: Name
  name: string
  description: string
  percent: Uint64
}

export interface Expfunds {
  id: Uint64
  coopname: Name
  contract: Name
  name: string
  description: string
  expended: Asset
}

export interface Fundwallet {
  id: Uint64
  coopname: Name
  circulating: Asset
  membership: Asset
  accumulated: Asset
  withdrawed: Asset
  available: Asset
  expended: Asset
}

export interface Fundwithdraw {
  coopname: Name
  username: Name
  type: Name
  fund_id: Uint64
  document: Document
  quantity: Asset
  bank_data_id: string
}

export interface Fwithdraws {
  id: Uint64
  coopname: Name
  username: Name
  status: Name
  type: Name
  fund_id: Uint64
  quantity: Asset
  document: Document
  bank_data_id: string
  expired_at: TimePointSec
}

export interface Init {
  coopname: Name
  initial: Asset
}

export interface Newfund {
  coopname: Name
  type: Name
  id: Uint64
}

export interface Newwithdraw {
  coopname: Name
  type: Name
  id: Uint64
}

export interface Spreadamount {
  coopname: Name
  quantity: Asset
}

export interface Subaccum {
  coopname: Name
  fund_id: Uint64
  quantity: Asset
}

export interface Subcirculate {
  coopname: Name
  quantity: Asset
}

