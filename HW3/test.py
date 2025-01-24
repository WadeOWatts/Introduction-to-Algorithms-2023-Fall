def count_permutations_recursive(n, N, current_sum=0, depth=0):
    # Base case: if current sum equals N, count this as one valid permutation
    if current_sum == N:
        return 1
    
    # If current sum exceeds N, this is not a valid permutation
    if current_sum > N:
        return 0

    # Count permutations by trying to add each number from 1 to n
    permutation_count = 0
    for i in range(1, n + 1):
        permutation_count += count_permutations_recursive(n, N, current_sum + i, depth + 1)

    return permutation_count

# Example usage
print(count_permutations_recursive(3, 7))  # Example with n = 3 and N = 7

