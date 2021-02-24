#include <iostream>

using namespace std;

template<class T, class Compare = less<T>>
class BST {
	class Node {
		friend class BST;

		T value;
		Node *parent;
		Node *left;
		Node *right;

		Node() :
			value(),
			parent(nullptr),
			left(nullptr),
			right(nullptr) {}

		Node(const T &value) :
			value(value),
			parent(nullptr),
			left(nullptr),
			right(nullptr) {}

		Node(const Node &node) :
			value(node->value),
			parent(node->parent),
			left(node->left),
			right(node->right) {}

		Node(const Node *node, const Node *parent = nullptr) : value(node->value), parent(parent) {
			left = new Node(node->left, this);
			right = new Node(node->right, this);
		}
	};

	template<class I>
	class Iterator : public iterator<bidirectional_iterator_tag, I> {
		friend class BST;
	private:
		Node * node;

		Node *getNode() {
			return node;
		}

	public:
		Iterator() : node(nullptr) {}

		Iterator(Node *node) : node(node) {}

		Iterator(const Iterator &it) : node(it.node) {}

		Iterator& operator++() {
			node = inc(node);
			return *this;
		}

		Iterator& operator--() {
			node = dec(node);
			return *this;
		}

		bool operator==(const Iterator &it) const {
			return (node == it.node);
		}

		bool operator!=(const Iterator &it) const {
			return !(node == it.node);
		}

		typename Iterator::reference operator*() const {
			if (node == nullptr)
				return NULL;
			return node->value;
		}
	};

private:
	Node *root;
	size_t treeSize;

	void clear(Node *node) {
		if (node == nullptr)
			return;

		clear(node->left);
		clear(node->right);
		delete node;
	}

	Node *beginNode() {
		Node *node = root;
		if (node == nullptr)
			return nullptr;
		while (node->left != nullptr)
			node = node->left;
		return node;
	}

	Node *endNode() {
		Node *node = root;
		if (node == nullptr)
			return nullptr;
		while (node->right != nullptr)
			node = node->right;
		return node;
	}

	static Node *inc(Node *node) {
		if (node->right == nullptr) {
			Node *curParent = node->parent;
			while (curParent != nullptr &&  node == curParent->right) {
				node = curParent;
				curParent = curParent->parent;
			}
			if (node->right != curParent || (node->right == nullptr && curParent == nullptr))
				node = curParent;
		}
		else {
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		return node;
	}

	static Node *dec(Node *node) {
		if (node->left == nullptr) {
			Node *curParent = node->parent;
			while (curParent != nullptr &&  node == curParent->left) {
				node = curParent;
				curParent = curParent->parent;
			}
			node = curParent;
		}
		else {
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}
		return node;
	}

	Node *next(const T &value) {
		Node *curNode = root;
		Node *nextNode = nullptr;
		while (curNode != nullptr) {
			if (Compare()(value, curNode->value)) {
				nextNode = curNode;
				curNode = curNode->left;
			}
			else
				curNode = curNode->right;
		}
		return nextNode;
	}

public:
	typedef Iterator<T> iterator;
	typedef Iterator<const T> const_iterator;

	BST() : root(nullptr), treeSize(0) {}

	BST(const BST &tree) {
		this = tree;
	}

	~BST() {
		clear(root);
	}

	BST &operator=(const BST &tree) {
		clear(root);
		root = new Node(tree.root);
		treeSize = tree.treeSize;
		return *this;
	}

	template <class InputIt>
	void assign(InputIt first, InputIt second) {
		for (InputIt it = first; it != second; ++it)
			insert(*it);
	}

	iterator begin() {
		return iterator(beginNode());
	}

	const_iterator cbegin() {
		return const_iterator(beginNode());
	}

	iterator end() {
		return iterator(endNode());
	}

	const_iterator cend() {
		return const_iterator(endNode());
	}

	iterator insert(const T &value) {
		Node *newNode = new Node(value);

		if (root == nullptr) {
			root = newNode;
			++treeSize;
			return iterator(root);
		}

		Node *curNode = root;
		while (curNode != nullptr) {
			if (Compare()(curNode->value, newNode->value)) {
				if (curNode->right != nullptr)
					curNode = curNode->right;
				else {
					newNode->parent = curNode;
					curNode->right = newNode;
					break;
				}
			}
			else if (Compare()(newNode->value, curNode->value)) {
				if (curNode->left != nullptr)
					curNode = curNode->left;
				else {
					newNode->parent = curNode;
					curNode->left = newNode;
					break;
				}
			}
			else return iterator(curNode);
		}
		++treeSize;
		return iterator(newNode);
	}

	iterator remove(const T &value) {
		Node *oldNode = find(value).getNode();
		Node *returned = inc(oldNode);

		if (oldNode == nullptr)
			return iterator();

		Node *oldParent = oldNode->parent;

		if (oldNode->left == nullptr && oldNode->right == nullptr) { //end level in tree
			if (oldNode == nullptr)
				root = nullptr;
			else {
				if (oldParent->left == oldNode)
					oldParent->left = nullptr;
				if (oldParent->right == oldNode)
					oldParent->right = nullptr;
			}
		}
		else if (oldNode->left == nullptr || oldNode->right == nullptr) { //pre-end level in tree
			if (oldParent == nullptr) {
				if (oldNode->left == nullptr) {
					root = oldNode->right;
					root->parent = nullptr;
				}
				else {
					root = oldNode->left;
					root->parent = nullptr;
				}
			}
			else {
				if (oldNode->left == nullptr) {
					if (oldParent->left == oldNode)
						oldParent->left = oldNode->right;
					if (oldParent->right == oldNode)
						oldParent->right = oldNode->right;
					oldNode->right->parent = oldParent;
				}
				else {
					if (oldParent->left == oldNode)
						oldParent->left = oldNode->left;
					if (oldParent->right == oldNode)
						oldParent->right = oldNode->left;
					oldNode->left->parent = oldParent;
				}
			}
		}
		else {
			//?parent
			Node *sucNode = next(value);
			oldNode->value = sucNode->value;
			if (sucNode->parent->left == sucNode) {
				sucNode->parent->left = sucNode->right;
				if (sucNode->right != nullptr)
					sucNode->right->parent = sucNode->parent;
			}
			else {
				sucNode->parent->right = sucNode->right;
				if (sucNode->right != nullptr)
					sucNode->right->parent = sucNode->parent;
			}
		}
		--treeSize;

		return iterator(returned);
	}

	iterator find(const T &value) {
		Node *curNode = root;
		while (curNode != nullptr && value != curNode->value) {
			if (Compare()(value, curNode->value))
				curNode = curNode->left;
			else
				curNode = curNode->right;
		}
		return iterator(curNode);
	}

	const_iterator find(const T &value) const {
		Node *curNode = root;
		while (curNode != nullptr && value != curNode->value) {
			if (Compare()(value, curNode->value))
				curNode = curNode->left;
			else
				curNode = curNode->right;
		}
		return const_iterator(curNode);
	}

	bool empty() {
		return (treeSize == 0);
	}

	size_t size() const {
		return treeSize;
	}

	void pr() {
		printTree(root, 0);
	}

	void printTree(Node *node, int level) {
		if (node != nullptr) {
			printTree(node->left, level + 1);
			for (int i = 0; i < level; i++)
				cout << "\t";
			cout << node->value << endl;
			printTree(node->right, level + 1);
		}
	}
};



int main() {
	BST<int> tree;
	string command;
	
	while (cin >> command) {
		int x;
		if (command == "insert") {
			cin >> x;
			tree.insert(x);
		}
		if (command == "remove") {
			cin >> x;
			cout << *tree.remove(x) << endl;
		}
		if (command == "print_all") {
			for (BST<int>::iterator it = tree.begin(); it != nullptr; ++it)
				cout << *it << ' ';
			cout << endl;
		}
		if (command == "size")
			cout << tree.size() << endl;
	}


	system("pause");
	return 0;
}