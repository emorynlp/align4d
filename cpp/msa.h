#ifndef MSA_MSA_H
#define MSA_MSA_H

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>

#define FULLY_MATCH_SCORE 2
#define PARTIAL_MATCH_SCORE 1
#define MISMATCH_SCORE (-1)
#define GAP_SCORE (-1)
#define GAP "-"

int edit_distance(const std::string &, const std::string &);

int compare(const std::string &, const std::vector<std::string> &);

void get_sequence_position_list_aux(const std::vector<int> &, int, int, std::vector<std::vector<int>> &, std::vector<int> &);

std::vector<std::vector<int>> get_sequence_position_list(int);

std::vector<std::vector<int>> get_current_index(const std::vector<int>&, const std::vector<int>&);

std::vector<std::vector<int>> get_parameter_index_list(const std::vector<int>&, const std::vector<int>&);

std::vector<std::string> get_compare_parameter(const std::vector<int>&, const std::vector<int>&, const std::vector<std::vector<std::string>>&);

size_t get_index(const std::vector<int>&, const std::vector<int>&);

std::vector<std::vector<std::string>> multi_sequence_alignment(const std::vector<std::string>&, const std::vector<std::vector<std::string>>&);

#endif //MSA_MSA_H
