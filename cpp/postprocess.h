#ifndef MSA_POSTPROCESS_H
#define MSA_POSTPROCESS_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

template <typename T> void write_csv_single_line(const std::string& file_name, const T& row) {
    /*
     * Write single row of content to a new or existing csv file
     *
     * @param file_name: path of the csv file
     * @param row: content need to be written in csv, type has to be 1d structure that support range-based for loop
     */
    std::ofstream file;
    file.open(file_name, std::ios::app);
    if (file.is_open()) {
        for (const auto& element: row) {
            file << element << ",";
        }
        file << std::endl;
    } else {
        throw std::runtime_error("Could not open the file");
    }
    file.close();
}

template <typename T> void write_csv_multiple_line(const std::string& file_name, const T& content) {
    /*
     * Write multiple rows of content to a new or existing csv file
     *
     * @param file_name: path of the csv file
     * @param row: content need to be written in csv, type has to be 2d structure that support range-based for loop
     */
    std::ofstream file;
    file.open(file_name, std::ios::app);
    if (file.is_open()) {
        for (const auto& row: content) {
            for (const auto& element: row) {
                file << element << ",";
            }
            file << std::endl;
        }
    } else {
        throw std::runtime_error("Could not open the file");
    }
    file.close();
}

void write_csv(const std::string&, const std::vector<std::vector<std::string>>&);

std::vector<std::string> get_token_match_result(const std::vector<std::vector<std::string>>&);

std::vector<std::vector<int>> get_align_indices(const std::vector<std::vector<std::string>>&);

std::vector<std::vector<int>> get_ref_original_indices(const std::vector<std::string>&, const std::vector<std::string>&);

std::vector<std::string> get_aligned_hypo_speaker_label(const std::vector<std::vector<std::string>>&, const std::vector<std::string>&);

#endif //MSA_POSTPROCESS_H
