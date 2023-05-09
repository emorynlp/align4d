//
// Created by peili on 2023/2/3.
//

#ifndef MSA_ALIGN_H
#define MSA_ALIGN_H

#include <string>
#include <vector>

std::vector<std::vector<std::string>> align_without_segment(const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&, int = 2);

std::vector<std::vector<std::string>> align_with_auto_segment(const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&, int = 2);

std::vector<std::vector<std::string>> align_with_manual_segment(const std::vector<std::string>&, const std::vector<std::string>&, const std::vector<std::string>&, int, int, int = 2);

std::vector<std::vector<std::string>> align_from_csv(const std::string&, int, int, int, int = 2);

#endif //MSA_ALIGN_H
