# User Instruction

## Introduction

Align4d is a powerful Python package used for aligning text results from Speaker Diarization and Speech Recognition. This user manual provides a step-by-step guide on how to install, use and troubleshoot the package.

## Installation

To install Align4d, you need to have Python version 3.10 or higher. Follow these steps:

1. Open your terminal or command prompt.
2. Type in the following command: `pip install align4d`
3. Wait for the package to download and install.

## Usage

### Importing Align4d

To use Align4d in your Python code, you need to import it. Here's how:

```python
from align4d import align4d

```

### Aligning Short Text Results

Align4d can align short text (less than 100 tokens) results from Speaker Diarization and Speech Recognition. Here's how to use it:

```python
aligned_results = align4d.align_without_segment(hypothesis, reference, reference_speaker_label)

```

The `align_without_segment()` function takes in three parameters:

1. `hypothesis`: This is a list of strings containing tokenized text . Each string represents a word that is generated from the Speech Recognition model. It is suggested to remove all the punctuations, escape values, and any other characters that is not in the natural language.
    
    ```python
    hypothesis = ["ok", "I", "am", "a", "fish", "Are", "you", "Hello", "there", "How", "are", "you", "ok"]
    ```
    
2. `reference`: This is a list of strings containing tokenized text from the gold standard text. Each string represents a word. It is suggested to remove all the punctuations, escape values, and any other characters that is not in the natural language.
    
    ```python
    reference = ["I", "am", "a", "fish", "ok", "Are", "you", "Hello", "there", "How", "are", "you"]
    ```
    
3. `reference_speaker_label`: This is a list of strings containing the speaker label from the gold standard text. Each string represents a word. It is suggested to remove all the punctuations, escape values, and any other characters that is not in the natural language.
    
    ```python
    reference_speaker_label = ["A", "A", "A", "A", "B", "C", "C", "D", "D", "E", "E", "E"]
    ```
    

The `align_without_segment()` function returns a nested list of strings containing the aligned results. The first list of strings is the hypothesis list. The rest of the lists are the reference lists separated according to the provided speaker label. In each list, each word is aligned to the positions that have the same index and the gap is denoted as “-”.

```python
align_result = align4d.align_without_segment(hypothesis, reference, reference_speaker_label)
for row in align_result:
    print(row)
```

Output from `align_without_segment()` : 

```python
# content in align_result
[['ok', 'I', 'am', 'a', 'fish', 'Are', 'you', 'Hello', 'there', 'How', 'are', 'you', 'ok']
['-', 'I', 'am', 'a', 'fish', '-', '-', '-', '-', '-', '-', '-', '-']
['ok', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-']
['-', '-', '-', '-', '-', 'Are', 'you', '-', '-', '-', '-', '-', '-']
['-', '-', '-', '-', '-', '-', '-', 'Hello', 'there', '-', '-', '-', '-']
['-', '-', '-', '-', '-', '-', '-', '-', '-', 'How', 'are', 'you', '-']]
```

At this stage, the alignment function will also print out the relative information for alignment calculation, including the size of the total matrix used for storing scores for alignment, the number of speakers, the maximum score in the matrix, and the time for computation.

```python
 matrix size: 14 5 2 3 3 4  total cell: 5040 speaker num: 5 cell max score: 24
time: 0
```

### Aligning Long Text Results

For longer text result from Speaker Diarization and Speech Recognition, we can use `align_with_auto_segment()` function. Under this situation, the `align_without_segment()` function may result in too long computation time and too large memory usage.

```
aligned_results = align4d.align_with_auto_segment(hypothesis, reference, reference_speaker_label)
```

The `align_with_auto_segment()` function requires the exact same parameters as the `align_without_segment()`. It performs automatic segmentation to slice the long text results into segments by detecting the absolute aligned parts between the hypothesis and reference sequences.

The output will be in the same form as the `align_without_segment()`. However, in addition to the information for alignment calculation, it will also print out the relative information for computing the optimal parameters for auto segmentation.

```python
segment length: 30 max hypothesis length: 13 max reference length: 12
segment length: 31 max hypothesis length: 13 max reference length: 12
segment length: 32 max hypothesis length: 13 max reference length: 12
...
...
segment length: 117 max hypothesis length: 13 max reference length: 12
segment length: 118 max hypothesis length: 13 max reference length: 12
segment length: 119 max hypothesis length: 13 max reference length: 12
optimal length: 119 optimal barrier length: 6
 matrix size: 14 5 2 3 3 4  total cell: 5040 speaker num: 5 cell max score: 24
time: 0
```

Output from `align_with_auto_segment()` : 

```python
# content in align_result
[['ok', 'I', 'am', 'a', 'fish', 'Are', 'you', 'Hello', 'there', 'How', 'are', 'you', 'ok']
['-', 'I', 'am', 'a', 'fish', '-', '-', '-', '-', '-', '-', '-', '-']
['ok', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-']
['-', '-', '-', '-', '-', 'Are', 'you', '-', '-', '-', '-', '-', '-']
['-', '-', '-', '-', '-', '-', '-', 'Hello', 'there', '-', '-', '-', '-']
['-', '-', '-', '-', '-', '-', '-', '-', '-', 'How', 'are', 'you', '-']]
```

### Aligning with Manual Segmentation

In case of `align_with_auto_segment()` failed during the automatic segmentation, you can manually tune the segmentation by specifying the parameters for segmentation using `align_with_manual_segment()`.

The `align_with_manual_segment()` requires five parameters, the first three are the `hypothesis`, `reference`, and `reference_speaker_label` in the exact same format. The rest two parameters are:

1. `segment_length`: This is a int that specifies the minimum length of each segment. For `align_with_auto_segment()` the program will search the optimal `segment_length` between 30 and 120.
2. `barrier_length`: This is a int that specifies the length of parts used to detect the absolute aligned parts. For `align_with_auto_segment()` the `barrier_length` is set to 6.

```python
align_result = align4d.align_with_manual_segment(hypo, ref, ref_speaker_label, 100, 6)
```

With manually specifying the parameters for segmentation, the program will not display the relative information for computing the optimal parameters for auto segmentation. The output will remain the same format as the `align_without_segment()`:

```python
 matrix size: 14 5 2 3 3 4  total cell: 5040 speaker num: 5 cell max score: 24
time: 0
```

```python
# content in align_result
[['ok', 'I', 'am', 'a', 'fish', 'Are', 'you', 'Hello', 'there', 'How', 'are', 'you', 'ok']
['-', 'I', 'am', 'a', 'fish', '-', '-', '-', '-', '-', '-', '-', '-']
['ok', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-', '-']
['-', '-', '-', '-', '-', 'Are', 'you', '-', '-', '-', '-', '-', '-']
['-', '-', '-', '-', '-', '-', '-', 'Hello', 'there', '-', '-', '-', '-']
['-', '-', '-', '-', '-', '-', '-', '-', '-', 'How', 'are', 'you', '-']]
```

## Troubleshooting

If you encounter any issues while using Align4d, try the following:

1. Make sure you have installed Python version 3.10 or higher.
2. Make sure you have installed the latest version of Align4d.
3. Check the input data to make sure it is in the correct format.
    1. The length of the `reference` and `reference_speaker_label` needs to be the same.
    2. All the input strings must be encoded in the utf-8 format.

## Conclusion

Align4d is a powerful Python package that can be used to align text results from Speaker Diarization and Speech Recognition. With this user manual, you should be able to install, use and troubleshoot the package with ease.
