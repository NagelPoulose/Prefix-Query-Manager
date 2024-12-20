#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This program is written by: Nagel Poulose

#define ALPHABET_SIZE 26

typedef struct TrieNode {
  int node_freq;
  int sum_prefix_freq;
  int max_child_freq;
  struct TrieNode *children[ALPHABET_SIZE];
} TrieNode;

// function that creates a new trie node
TrieNode *create_node() {
  TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
  node->node_freq = 0;
  node->sum_prefix_freq = 0;
  node->max_child_freq = 0;
  for (int i = 0; i < ALPHABET_SIZE; i++)
    node->children[i] = NULL;
  return node;
}

// function that inserts
void insert(TrieNode *root, char *word, int freq) {
  TrieNode *node = root;
  node->sum_prefix_freq += freq;
  for (int i = 0; word[i]; i++) {
    int index = word[i] - 'a';
    if (!node->children[index])
      node->children[index] = create_node();
    node = node->children[index];
    node->sum_prefix_freq += freq;
  }
  node->node_freq += freq;

  // this part updates max_child_freq as it goes along the path
  node = root;
  for (int i = 0; word[i]; i++) {
    int index = word[i] - 'a';
    int max_freq = 0;
    for (int j = 0; j < ALPHABET_SIZE; j++) {
      if (node->children[j] && node->children[j]->sum_prefix_freq > max_freq)
        max_freq = node->children[j]->sum_prefix_freq;
    }
    node->max_child_freq = max_freq;
    node = node->children[index];
  }
}

// the query function
void query(TrieNode *root, char *prefix) {
  TrieNode *node = root;
  for (int i = 0; prefix[i]; i++) {
    int index = prefix[i] - 'a';
    if (!node->children[index]) {
      printf("unrecognized prefix\n");
      return;
    }
    node = node->children[index];
  }
  // if the prefix is itself a word and doesnt have any children
  int has_child = 0;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (node->children[i]) {
      has_child = 1;
      break;
    }
  }
  if (!has_child) {
    printf("unrecognized prefix\n");
    return;
  }

  // finds letters with the max frequency
  int max_freq = node->max_child_freq;
  char result[ALPHABET_SIZE + 1];
  int res_index = 0;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    if (node->children[i] && node->children[i]->sum_prefix_freq == max_freq) {
      result[res_index++] = 'a' + i;
    }
  }
  result[res_index] = '\0';

  // sorts the results in alphabetical order
  for (int i = 0; i < res_index - 1; i++) {
    for (int j = i + 1; j < res_index; j++) {
      if (result[i] > result[j]) {
        char temp = result[i];
        result[i] = result[j];
        result[j] = temp;
      }
    }
  }
  printf("%s\n", result);
}

void free_trie(TrieNode *node) {
  if (node == NULL)
    return;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    free_trie(node->children[i]);
  }
  free(node);
}

// main function that processes that takes in the commands and calls the
// functions
int main() {
  int n;
  scanf("%d", &n);
  TrieNode *root = create_node();
  for (int i = 0; i < n; i++) {
    int cmd;
    scanf("%d", &cmd);
    if (cmd == 1) {
      char word[100001];
      int freq;
      scanf("%s %d", word, &freq);
      insert(root, word, freq);
    } else if (cmd == 2) {
      char prefix[100001];
      scanf("%s", prefix);
      query(root, prefix);
    }
  }
  free_trie(root);
  return 0;
}
