def align_without_segment(hypothesis: list[str], reference: list[str], reference_label: list[str]) -> list[list[str]]:
    """

    :param hypothesis:
    :param reference:
    :param reference_label:
    :return:
    """
    pass


def align_with_auto_segment(hypothesis: list[str], reference: list[str], reference_label: list[str]) -> list[list[str]]:
    pass


def align_with_manual_segment(hypothesis: list[str], reference: list[str], reference_label: list[str],
                              segment_length: int, barrier_length: int) -> list[list[str]]:
    pass


def get_token_match_result(align_result: list[list[str]]) -> list[str]:
    pass


def get_align_indices(align_result: list[list[str]]) -> list[list[int]]:
    pass


def get_ref_original_indices(reference: list[str], reference_label: list[str]) -> list[list[int]]:
    pass
