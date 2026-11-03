// Copyright (c) 2026 The GlobalBoost-Y developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <key_mldsa_address.h>

namespace wallet {

CMldsaAddress::CMldsaAddress(const CMldsaPublicKey& pubkey)
    : m_pubkey(pubkey)
{
}

bool CMldsaAddress::IsValid() const
{
    // Scaffold only. Real implementation must validate canonical address format.
    return m_pubkey.IsValid();
}

std::string CMldsaAddress::ToString() const
{
    // Scaffold only. Real implementation must emit canonical address bytes/encoding.
    return "";
}

bool CMldsaAddress::FromString(const std::string& str)
{
    // Scaffold only. Real implementation must parse and validate the format.
    (void)str;
    return false;
}

CMldsaPublicKey CMldsaAddress::GetPubKey() const
{
    return m_pubkey;
}

void CMldsaAddress::SetPubKey(const CMldsaPublicKey& pubkey)
{
    m_pubkey = pubkey;
}

} // namespace wallet
