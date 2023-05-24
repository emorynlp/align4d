import copy
import string
import os
import shutil
import subprocess
import glob
import sys

from align4d import align4d


def align(hypothesis: str | list[str], reference: list[list], partial_bound: int = 2, segment_length: int = None,
          barrier_length: int = None, strip_punctuation: bool = True) -> dict:
    # pre-processing
    if type(hypothesis) == str:
        hypothesis_temp = hypothesis.split()
    else:
        hypothesis_temp = copy.deepcopy(hypothesis)
    reference_temp = []
    reference_label = []
    for utterance in reference:
        if len(utterance) == 1:
            if type(utterance[0]) == str:
                reference_temp.extend(utterance[0].split())
                reference_label.extend(["A"] * len(utterance[0].split()))
            elif type(utterance[0]) == list:
                reference_temp.extend(utterance[0])
                reference_label.extend(["A"] * len(utterance[0]))
        elif len(utterance) == 2:
            if type(utterance[1]) == str:
                reference_temp.extend(utterance[1].split())
                reference_label.extend([utterance[0]] * len(utterance[1].split()))
            elif type(utterance[1]) == list:
                reference_temp.extend(utterance[1])
                reference_label.extend([utterance[0]] * len(utterance[1]))
    if strip_punctuation:
        TRANS = str.maketrans('', '', string.punctuation)
        hypothesis_strip = [s.translate(TRANS) for s in hypothesis_temp]
        reference_strip = [s.translate(TRANS) for s in reference_temp]
    else:
        hypothesis_strip = hypothesis_temp
        reference_strip = reference_temp

    # align
    if (segment_length is None and barrier_length is not None) or (barrier_length is None and segment_length is not None):
        raise Exception("Segment length or barrier length parameter incorrect or missing.")
    if segment_length is None and barrier_length is None:
        if len(hypothesis) < 100:
            align_result = align4d.align_without_segment(hypothesis_strip, reference_strip, reference_label, partial_bound)
        else:
            align_result = align4d.align_with_auto_segment(hypothesis_strip, reference_strip, reference_label, partial_bound)
    elif segment_length <= 0 and barrier_length <= 0:
        align_result = align4d.align_without_segment(hypothesis_strip, reference_strip, reference_label, partial_bound)
    elif segment_length > 0 and barrier_length > 0:
        align_result = align4d.align_with_manual_segment(hypothesis_strip, reference_strip, reference_label, segment_length, barrier_length, partial_bound)
    else:
        raise Exception("Segment length or barrier length parameter incorrect or missing.")

    # post-processing
    unique_speaker_label = align4d.get_unique_speaker_label(reference_label)
    if strip_punctuation:
        # for hypothesis
        hypo_index = 0
        for i in range(len(align_result[0])):
            if align_result[0][i] != '-':
                align_result[0][i] = hypothesis_temp[hypo_index]
                hypo_index += 1
        # for reference
        output_index = [0 for _ in range(len(align_result))]
        for i in range(len(reference_temp)):
            speaker_index = unique_speaker_label.index(reference_label[i]) + 1
            while output_index[speaker_index] < len(align_result[0]) and align_result[speaker_index][output_index[speaker_index]] == '-':
                output_index[speaker_index] += 1
            align_result[speaker_index][output_index[speaker_index]] = reference_temp[i]
            output_index[speaker_index] += 1
    align_result = [[s if s != '-' else '' for s in seq] for seq in align_result]
    output = {"hypothesis": align_result[0], "reference": {}}
    for i in range(len(unique_speaker_label)):
        output["reference"][f"{unique_speaker_label[i]}"] = align_result[i + 1]
    return output


def token_match(output: dict, partial_bound: int = 2) -> list[str]:
    align_result = [output["hypothesis"]]
    for value in output["reference"].values():
        align_result.append(value)
    TRANS = str.maketrans('', '', string.punctuation)
    align_result = [[token.translate(TRANS) for token in row] for row in align_result]
    align_result = [[token if token != '' else '-' for token in row] for row in align_result]
    return align4d.get_token_match_result(align_result, partial_bound)


def align_indices(output: dict) -> dict:
    align_result = [output["hypothesis"]]
    for value in output["reference"].values():
        align_result.append(value)
    TRANS = str.maketrans('', '', string.punctuation)
    align_result = [[token.translate(TRANS) for token in row] for row in align_result]
    align_result = [[token if token != '' else '-' for token in row] for row in align_result]
    align_indices_list = align4d.get_align_indices(align_result)
    align_indices = {}
    for index, speaker_label in enumerate(output["reference"].keys()):
        align_indices[speaker_label] = align_indices_list[index]
    return align_indices


def compile():
    # Get the current script's directory
    script_dir = os.path.dirname(os.path.realpath(__file__))

    # Path to 'cpp' directory
    cpp_dir = os.path.join(script_dir, 'cpp')

    # Path to 'build' directory
    build_dir = os.path.join(cpp_dir, 'build')

    # Check if 'build' directory exists, remove it if it does
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

    # Check for files with extension '.so', '.pyd', or '.dll' in 'cpp' directory and remove them
    for ext in ['*.so', '*.pyd', '*.dll']:
        for f in glob.glob(os.path.join(script_dir, ext)):
            os.chmod(f, 0o777)
            os.remove(f)

    # Change to 'cpp' directory
    os.chdir(cpp_dir)

    # Run 'setup.py build' command
    subprocess.check_call([sys.executable, 'setup.py', 'build'])

    # Find directory that contains 'lib' in its name
    lib_dir = next((os.path.join(build_dir, d) for d in os.listdir(build_dir) if 'lib' in d), None)
    if lib_dir is None:
        raise FileNotFoundError('No directory containing "lib" was found in the build directory.')

    # Find file with extension '.so', '.pyd', or '.dll'
    file_path = next(
        (os.path.join(lib_dir, f) for ext in ['*.so', '*.pyd', '*.dll'] for f in glob.glob(os.path.join(lib_dir, ext))),
        None)
    if file_path is None:
        raise FileNotFoundError('No file with extension .so, .pyd or .dll was found.')

    # Copy the found file to the parent directory of the 'cpp' directory
    shutil.copy(file_path, script_dir)

    # Check if 'build' directory exists, remove it if it does
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)

