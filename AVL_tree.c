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


void R_rotation(struct tree** node) {  // правий поворот
    struct tree* temp = (*node)->left;

    (*node)->left = temp->right;
    temp->right = *node;
    *node = temp;
}

void L_rotation(struct tree** node) {  // лівий поворот
    struct tree* temp = (*node)->right;

    (*node)->right = temp->left;
    temp->left = *node;
    *node = temp;
}

void RL_rotation(struct tree** node) {  // право-лівий поворот
    R_rotation(&((*node)->right));
    L_rotation(node);
}

void LR_rotation(struct tree** node) {  // ліво-правий поворот
    L_rotation(&((*node)->left));
    R_rotation(node);
}

int height(struct tree* node) {  // висота дерева
    if (!node) {
        return 0;
    }
    return max(height(node->left), height(node->right)) + 1;
}

int balance_factor(struct tree* node) {  // показник збалансованості
    return height(node->left) - height(node->right);
}

void balance(struct tree** node) {  // балансування
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

void insert(struct tree** root, int el) {  // додавання елементу
    struct tree* temp = *root, *node;

    node = (struct tree*) malloc(sizeof(struct tree));  // ініціалізація вершини, що буде додана
    node->element = el;
    node->left = NULL;
    node->right = NULL;

    if (*root == NULL) {  // випадок, коли дерево порожнє
        *root = node;
        balance(root);
    } else {  // випадок, коли дерево не порожнє
        while (temp) {  // спуск по дереву до потрібної позиції
            if (el < temp->element) {
                if (temp->left == NULL) {
                    temp->left = node;  // вставка елементу
                    balance(root);  // балансування
                    return;
                }
                temp = temp->left;
            } else if (el > temp->element) {
                if (temp->right == NULL) {
                    temp->right = node;  // вставка елементу
                    balance(root);  // балансування
                    return;
                }
                temp = temp->right;
            } else {
                return;
            }
        }
    }
}

void delete_helper(struct tree** node, int el) {  // допоміжна функція видалення
    struct tree* temp = *node;

    if ((*node)->element == el) {
        if ((*node)->left == NULL && (*node)->right == NULL) { // випадок, коли елемент, що видаляється, не має нащадків (листок)
            *node = NULL;
            free(temp);
        } else if ((*node)->left == NULL) { // випадок, коли елемент, що видаляється, має лише правого нащадка
            *node = (*node)->right;
            free(temp);
        } else if ((*node)->right == NULL) {  // випадок, коли елемент, що видаляється, має лише лівого нащадка
            *node = (*node)->left;
            free(temp);
        } else {  // випадок, коли елемент, що видаляється, має двох нащадків
            temp = temp->right;  // спуск до мінімального елемента правого піддерева
            if (temp->left == NULL) {
                temp->left = (*node)->left;
                free(*node);
                *node = temp;
            } else {
                while (temp->left->left != NULL) {
                    temp = temp->left;
                }
                struct tree* memory = temp->left;  // перестановки та очищення пам'яті
                temp->left = memory->right;
                memory->left = (*node)->left;
                memory->right = (*node)->right;
                free(*node);
                *node = memory;
            }
        }
    }
}

void delete(struct tree** root, int el) {  // основна функція видалення
    struct tree* node = *root;

    if ((*root)->element == el) {  // випадок, коли елемент, що видаляється, - корінь
        delete_helper(root, el);
        balance(root);
    } else {
        while (node->element != el) {  // спуск по дереву до елемента, що видаляється
            if (el < node->element) {
                if (node->left->element == el) {
                    delete_helper(&(node->left), el);  // видалення елементу
                    balance(root);  // балансування
                    break;
                }
                node = node->left;
            } else if (el > node->element) {
                if (node->right->element == el) {
                    delete_helper(&(node->right), el);  // видалення елементу
                    balance(root);  // балансування
                    break;
                }
                node = node->right;
            }
        }
    }
}

void show_tree_helper(struct tree* root, int height) {  // допоміжна функція виведення дерева
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

void show_tree(struct tree* root) {  // основна функція виведення дерева
    show_tree_helper(root, 0);
}

int main(void) {

    struct tree* root = NULL;  // ініціалізація пустого дерева

    insert(&root, 50);    // додавання елементів
    insert(&root, 100);
    insert(&root, 150);
    insert(&root, 25);
    insert(&root, 10);
    insert(&root, 200);
    insert(&root, 45);
    show_tree(root);      // виведення дерева

    delete(&root, 50);    // видалення елементів
    delete(&root, 100);
    delete(&root, 10);
    delete(&root, 45);
    show_tree(root);      // виведення дерева


    return 0;
}
