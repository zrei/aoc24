<h1>Part 1</h1>
Used something similar to a sliding window approach with a left pointer that starts from the front and a right pointer that starts from the end, moving occupied spaces from the right pointer to empty spaces at the left pointer.

Iterate through the reconstructed line to get the final checksum value.

<h1>Part 2</h1>
Brute-forced this part. Starting from the back, find the next occupied file block, and then check for empty space starting from the left. If the empty space is large enough for the file block, move the file block over. Otherwise, check for the next empty space, or stop if there is no more. The right pointer's position is remembered between each iteration, and the execution can stop once the right pointer has moved past the first character in the line.