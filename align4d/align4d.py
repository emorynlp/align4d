def align_without_segment(hypothesis: list[str], reference: list[str], reference_label: list[str],
                          partial_bound: int = 2) -> list[list[str]]:
    pass


def align_with_auto_segment(hypothesis: list[str], reference: list[str], reference_label: list[str],
                            partial_bound: int = 2) -> list[list[str]]:
    pass


def align_with_manual_segment(hypothesis: list[str], reference: list[str], reference_label: list[str],
                              segment_length: int, barrier_length: int, partial_bound: int = 2) -> list[list[str]]:
    pass


def get_token_match_result(align_result: list[list[str]], partial_bound: int = 2) -> list[str]:
    pass


def get_align_indices(align_result: list[list[str]]) -> list[list[int]]:
    pass


def get_ref_original_indices(reference: list[str], reference_label: list[str]) -> list[list[int]]:
    pass


def get_unique_speaker_label(speaker_label: list[str]) -> list[str]:
    pass


def get_aligned_hypo_speaker_label(align_result: list[list[str]], hypothesis_label: list[str]) -> list[str]:
    pass
