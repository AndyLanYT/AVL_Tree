#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return a > b ? a : b;
}

struct tree {
    int element;
    struct tree* left;
    struct tree* right;
};


void R_rotation(struct tree** node) {
    struct tree* temp = (*node)->left;

    (*node)->left = temp->right;
    temp->right = *node;
    *node = temp;
}

void L_rotation(struct tree** node) {
    struct tree* temp = (*node)->right;

    (*node)->right = temp->left;
    temp->left = *node;
    *node = temp;
}

void RL_rotation(struct tree** node) {
    R_rotation(&((*node)->right));
    L_rotation(node);
}

void LR_rotation(struct tree** node) {
    L_rotation(&((*node)->left));
    R_rotation(node);
}

int height(struct tree* node) {
    if (!node) {
        return 0;
    }
    return max(height(node->left), height(node->right)) + 1;
}

int balance_factor(struct tree* node) {
    return height(node->left) - height(node->right);
}

void balance(struct tree** node) {
    if (!*node) {
        return;
    }
    if (balance_factor(*node) > 1) {
        if (balance_factor((*node)->left) >= 0) {
            R_rotation(node);
        } else {
            LR_rotation(node);
        }
    } else if (balance_factor(*node) < -1) {
        if (balance_factor((*node)->right) <= 0) {
            L_rotation(node);
        } else {
            RL_rotation(node);
        }
    }
    balance(&((*node)->left));
    balance(&((*node)->right));
}

void insert(struct tree** root, int el) {
    struct tree* temp = *root, *node;

    node = (struct tree*) malloc(sizeof(struct tree));
    node->element = el;
    node->left = NULL;
    node->right = NULL;

    if (*root == NULL) {
        *root = node;
        balance(root);
    } else {
        while (temp) {
            if (el < temp->element) {
                if (temp->left == NULL) {
                    temp->left = node;
                    balance(root);
                    return;
                }
                temp = temp->left;
            } else if (el > temp->element) {
                if (temp->right == NULL) {
                    temp->right = node;
                    balance(root);
                    return;
                }
                temp = temp->right;
            } else {
                return;
            }
        }
    }
}

void delete_helper(struct tree** node, int el) {
    struct tree* temp = *node;

    if ((*node)->element == el) {
        if ((*node)->left == NULL && (*node)->right == NULL) {
            *node = NULL;
            free(temp);
        } else if ((*node)->left == NULL) {
            *node = (*node)->right;
            free(temp);
        } else if ((*node)->right == NULL) {
            *node = (*node)->left;
            free(temp);
        } else {
            temp = temp->right;
            if (temp->left == NULL) {
                temp->left = (*node)->left;
                free(*node);
                *node = temp;
            } else {
                while (temp->left->left != NULL) {
                    temp = temp->left;
                }
                struct tree* subtree = temp->left;
                temp->left = subtree->right;
                subtree->left = (*node)->left;
                subtree->right = (*node)->right;
                free(*node);
                *node = subtree;
            }
        }
    }
}

void delete(struct tree** root, int el) {
    struct tree* temp = *root;

    if ((*root)->element == el) {
        delete_helper(root, el);
        balance(root);
    } else {
        while (temp->element != el) {
            if (el < temp->element) {
                if (temp->left->element == el) {
                    delete_helper(&(temp->left), el);
                    balance(root);
                    return;
                }
                temp = temp->left;
            } else if (el > temp->element) {
                if (temp->right->element == el) {
                    delete_helper(&(temp->right), el);
                    balance(root);
                    return;
                }
                temp = temp->right;
            }
        }
    }
}

void show_tree_helper(struct tree* root, int height) {
    if (!root) {
        return;
    }
    for (int i = 0; i < height+1; i++) {
        printf("     ");
    }
    printf("%d\n", root->element);
    show_tree_helper(root->left, height+1);
    show_tree_helper(root->right, height+1);
}

void show_tree(struct tree* root) {
    show_tree_helper(root, 0);
}
