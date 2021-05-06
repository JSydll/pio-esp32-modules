#include "esp32-modules/filesystem/SdCard.hpp"

namespace Esp32Modules::Filesystem
{
SdCard::SdCard() { mCardType = (SD.begin() ? SD.cardType() : CARD_UNKNOWN); }

SdCard::~SdCard() { SD.end(); }

bool SdCard::IsAvailable() const { return (mCardType != CARD_NONE and mCardType != CARD_UNKNOWN); }

fs::FS& SdCard::GetFilesystemHandle() { return SD; }

size_t SdCard::GetByteSize() const { return SD.cardSize(); }

size_t SdCard::GetBytesAvailable() const { return (SD.totalBytes() - SD.usedBytes()); }

}  // namespace Esp32Modules::Filesystem