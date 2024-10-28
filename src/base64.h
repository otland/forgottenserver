#ifndef TFS_BASE64_H
#define TFS_BASE64_H

namespace tfs::base64 {

std::string encode(std::string_view input);
std::string decode(std::string_view input);

} // namespace tfs::base64

#endif
