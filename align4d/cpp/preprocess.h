#ifndef MSA_PROCESSTEXT_H
#define MSA_PROCESSTEXT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

std::vector<std::vector<std::string>> read_csv(const std::string&);

std::vector<std::string> get_total_hypothesis(const std::vector<std::vector<std::string>>&, int);

std::vector<std::vector<std::string>> get_total_reference_with_label(const std::vector<std::vector<std::string>>&, int, int);

std::vector<std::string> get_unique_speaker_label(const std::vector<std::string>&);

std::vector<std::vector<int>> get_segment_index(const std::vector<std::string>&, const std::vector<std::string>&, int, int);

std::vector<std::vector<std::string>> get_segment_sequence(const std::vector<std::string>&, const std::vector<int>&);

std::vector<std::vector<std::string>> get_separate_sequence(const std::vector<std::string>&, const std::vector<std::string>&);

std::vector<std::vector<std::string>> get_separate_sequence_with_label(const std::vector<std::string>&, const std::vector<std::string>&);

void test_segment_parameter(int, int, int, const std::vector<std::string>&, const std::vector<std::string>&);

std::tuple<int, int> get_optimal_segment_parameter(const std::vector<std::string>&, const std::vector<std::string>&, int = 30, int = 120, int = 6);

#endif //MSA_PROCESSTEXT_H
