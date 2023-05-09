#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "align.h"
#include "msa.h"
#include "preprocess.h"
#include "postprocess.h"

std::vector<std::vector<std::string>> align_without_segment(const std::vector<std::string>& hypothesis, const std::vector<std::string>& reference, const std::vector<std::string>& reference_label, int partial_bound) {
    // get unique speaker labels
    std::vector<std::string> unique_speaker_label = get_unique_speaker_label(reference_label);
    // separate reference to multiple sequences by speaker label
    std::vector<std::vector<std::string>> separated_ref = get_separate_sequence(reference, reference_label);
    // align
    auto start = std::chrono::high_resolution_clock::now();
    auto align_result = multi_sequence_alignment(hypothesis, separated_ref, partial_bound);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "\ntime: " << duration.count() << std::endl;
    return align_result;
}

std::vector<std::vector<std::string>> align_with_auto_segment(const std::vector<std::string>& hypothesis, const std::vector<std::string>& reference, const std::vector<std::string>& reference_label, int partial_bound) {
    // get unique speaker labels
    std::vector<std::string> unique_speaker_label = get_unique_speaker_label(reference_label);

    // segment dialogue
    auto [optimal_segment_length, optimal_barrier_length] = get_optimal_segment_parameter(hypothesis, reference);
    std::vector<std::vector<int>> segment_index = get_segment_index(hypothesis, reference, optimal_segment_length, optimal_barrier_length);
    std::vector<std::vector<std::string>> segmented_hypothesis_list = get_segment_sequence(hypothesis, segment_index[0]);
    std::vector<std::vector<std::string>> segmented_reference_list = get_segment_sequence(reference, segment_index[1]);
    std::vector<std::vector<std::string>> segmented_reference_label_list = get_segment_sequence(reference_label, segment_index[1]);

    // align each segment separately, record time, and put all back together
    std::vector<std::vector<std::string>> align_result(unique_speaker_label.size() + 1);
    long long total_time{0};
    for (int i = 0; i < segmented_hypothesis_list.size(); ++i) {
        std::cout << " segment from: " << segment_index[0][i] << " to: " << segment_index[0][i + 1];
        auto segment_hypothesis = segmented_hypothesis_list[i];
        auto separated_reference_with_label = get_separate_sequence_with_label(segmented_reference_list[i], segmented_reference_label_list[i]);
        std::vector<std::vector<std::string>> separated_reference(separated_reference_with_label.begin(), separated_reference_with_label.end() - 1);
        std::vector<std::string> segment_reference_speaker_label = separated_reference_with_label.back();

        auto start = std::chrono::high_resolution_clock::now();
        auto result = multi_sequence_alignment(segment_hypothesis, separated_reference, partial_bound);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        std::cout << " segment time: " << duration.count() << std::endl;
        total_time += duration.count();

        align_result[0].insert(align_result[0].end(), result[0].begin(), result[0].end());
        for (int j = 0; j < separated_reference.size(); ++j) {
            int final_result_index = std::ranges::find(unique_speaker_label, segment_reference_speaker_label[j]) - unique_speaker_label.begin() + 1;
            align_result[final_result_index].insert(align_result[final_result_index].end(), result[j + 1].begin(), result[j + 1].end());
        }
        for (int j = 1; j < align_result.size(); ++j) {
            while (align_result[j].size() < align_result[0].size()) {
                align_result[j].emplace_back(GAP);
            }
        }
    }
    std::cout << "total time: " << total_time << std::endl;
    return align_result;
}

std::vector<std::vector<std::string>> align_with_manual_segment(const std::vector<std::string>& hypothesis, const std::vector<std::string>& reference, const std::vector<std::string>& reference_label, int segment_length, int barrier_length, int partial_bound) {
    // get unique speaker labels
    std::vector<std::string> unique_speaker_label = get_unique_speaker_label(reference_label);

    // segment dialogue
    std::vector<std::vector<int>> segment_index = get_segment_index(hypothesis, reference, segment_length, barrier_length);
    std::vector<std::vector<std::string>> segmented_hypothesis_list = get_segment_sequence(hypothesis, segment_index[0]);
    std::vector<std::vector<std::string>> segmented_reference_list = get_segment_sequence(reference, segment_index[1]);
    std::vector<std::vector<std::string>> segmented_reference_label_list = get_segment_sequence(reference_label, segment_index[1]);

    // align each segment separately, record time, and put all back together
    std::vector<std::vector<std::string>> align_result(unique_speaker_label.size() + 1);
    long long total_time{0};
    for (int i = 0; i < segmented_hypothesis_list.size(); ++i) {
        std::cout << " segment from: " << segment_index[0][i] << " to: " << segment_index[0][i + 1];
        auto segment_hypothesis = segmented_hypothesis_list[i];
        auto separated_reference_with_label = get_separate_sequence_with_label(segmented_reference_list[i], segmented_reference_label_list[i]);
        std::vector<std::vector<std::string>> separated_reference(separated_reference_with_label.begin(), separated_reference_with_label.end() - 1);
        std::vector<std::string> segment_reference_speaker_label = separated_reference_with_label.back();

        auto start = std::chrono::high_resolution_clock::now();
        auto result = multi_sequence_alignment(segment_hypothesis, separated_reference, partial_bound);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        std::cout << " segment time: " << duration.count() << std::endl;
        total_time += duration.count();

        align_result[0].insert(align_result[0].end(), result[0].begin(), result[0].end());
        for (int j = 0; j < separated_reference.size(); ++j) {
            int final_result_index = std::ranges::find(unique_speaker_label, segment_reference_speaker_label[j]) - unique_speaker_label.begin() + 1;
            align_result[final_result_index].insert(align_result[final_result_index].end(), result[j + 1].begin(), result[j + 1].end());
        }
        for (int j = 1; j < align_result.size(); ++j) {
            while (align_result[j].size() < align_result[0].size()) {
                align_result[j].emplace_back(GAP);
            }
        }
    }
    std::cout << "total time: " << total_time << std::endl;
    return align_result;
}

std::vector<std::vector<std::string>> align_from_csv(const std::string& input_file, int hypo_line, int ref_line, int ref_label_line, int partial_bound) {
    std::vector<std::vector<std::string>> content = read_csv(input_file);
    std::vector<std::string> hypothesis = get_total_hypothesis(content, hypo_line);
    std::vector<std::vector<std::string>> reference_with_label = get_total_reference_with_label(content, ref_line, ref_label_line);
    std::vector<std::string> reference = reference_with_label[0];
    std::vector<std::string> reference_label = reference_with_label[1];
    std::vector<std::vector<std::string>> align_result = align_with_auto_segment(hypothesis, reference, reference_label, partial_bound);
    return align_result;
}

int main() {
//    std::string file_name{"../data/example.csv"};
//    std::vector<std::vector<std::string>> content = read_csv(file_name);
//    std::vector<std::string> hypothesis = get_total_hypothesis(content, 0);
//    std::vector<std::vector<std::string>> reference_with_label = get_total_reference_with_label(content, 3, 4);
//    std::vector<std::string> reference = reference_with_label[0];
//    std::vector<std::string> reference_label = reference_with_label[1];
//
//    auto result = align_with_auto_segment(hypothesis, reference, reference_label);
//
//    auto token_match_result = get_token_match_result(result);
//    auto align_indexes = get_align_indices(result);
//    auto ref_original_index = get_ref_original_indices(reference, reference_label);
//    auto unique_speaker_label = get_unique_speaker_label(reference_label);
//    auto aligned_hypo_speaker_label = get_aligned_hypo_speaker_label(result, content[1]);
//
//    write_csv_multiple_line<std::vector<std::vector<std::string>>>("../data/example_output3.csv", result);
//    write_csv_single_line<std::vector<std::string>>("../data/example_output3.csv", token_match_result);
//    write_csv_multiple_line<std::vector<std::vector<int>>>("../data/example_output3.csv", align_indexes);
//    write_csv_multiple_line("../data/example_output3.csv", ref_original_index);
//    write_csv_single_line("../data/example_output3.csv", unique_speaker_label);
//    write_csv_single_line("../data/example_output3.csv", aligned_hypo_speaker_label);


//    std::vector<std::string> hypo{"ok", "I", "am", "a", "fish", "Are", "you", "Hello", "there", "How", "are", "you", "ok"};
//    std::vector<std::string> ref{"I", "am", "a", "fish", "okay", "Are", "you", "Hello", "there", "How", "are", "you"};
//    std::vector<std::string> ref_speaker_labels{"B", "B", "B", "B", "A", "C", "C", "D", "D", "E", "E", "E"};
//    std::vector<std::vector<std::string>> separated_ref = get_separate_sequence(ref, ref_speaker_labels);
//
//    auto result = align_with_auto_segment(hypo, ref, ref_speaker_labels, 3);
//
//    auto token_match_result = get_token_match_result(result, 3);
//    auto align_indexes = get_align_indices(result);
//    auto ref_original_index = get_ref_original_indices(ref, ref_speaker_labels);
//
//    for (const auto& a: result) {
//        for (const auto& b: a) {
//            std::cout << b << " ";
//        }
//        std::cout << std::endl;
//    }
//
//    for (const auto& b: token_match_result) {
//        std::cout << b << " ";
//    }
//    std::cout << std::endl;

//    write_csv_multiple_line<std::vector<std::vector<std::string>>>("../data/small_test.csv", result);
//    write_csv_single_line<std::vector<std::string>>("../data/small_test.csv", token_match_result);
//    write_csv_multiple_line<std::vector<std::vector<int>>>("../data/small_test.csv", align_indexes);
//    write_csv_multiple_line("../data/small_test.csv", ref_original_index);

    return 0;
}