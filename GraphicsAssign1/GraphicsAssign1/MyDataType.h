#pragma once
#define MY_RED MyColor(255, 0, 0)
#define MY_GREEN MyColor(0, 255, 0)
#define MY_BLUE MyColor(0, 0, 255)
#define MY_MAGENTA MyColor(255, 0, 255)
#define MY_YELLOW MyColor(255, 255, 0)
#define MY_CYAN MyColor(0, 255, 255)
#define MY_CLOUD_COLOR MyColor(152, 255, 255)
using namespace std;
struct Vector2 {
public:
	GLdouble x;
	GLdouble y;
	Vector2(GLdouble _x, GLdouble _y) {
		x = _x;
		y = _y;
	}

	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2 operator+(const Vector2& rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	Vector2 operator-(const Vector2& rhs) {
		return Vector2(x - rhs.x, y - rhs.y);
	}
	Vector2 operator*(const GLdouble d) {
		return Vector2((*this).x * d, (*this).y * d);
	}
	Vector2 operator/(const GLdouble d) {
		return Vector2((*this).x / d, (*this).y / d);
	}
	Vector2 operator-() const {
		Vector2 v;
		v.x = -x;
		v.y = -y;
		return v;
	}
	Vector2& operator+=(const Vector2& rhs) {
		(*this).x += rhs.x;
		(*this).y += rhs.y;
		return *this;
	}

	inline static GLdouble abs(const Vector2 & a)
	{
		return sqrt(a.x*a.x + a.y*a.y);
	}

	inline static Vector2 normalize(const Vector2& a) {
		GLdouble veclen = sqrt(a.x*a.x + a.y*a.y);
		return Vector2(a.x / veclen, a.y / veclen);
	}
	friend ostream& operator<<(ostream& os, const Vector2& dt) {
		os << '(' << dt.x << ',' << dt.y << ')';
		return os;
	}
};
struct MyColor {
public:
	int r;
	int g;
	int b;
	double a;
	MyColor(int _r, int _g, int _b, double _a = 1) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
};

template<typename T, typename Key>
class Node {
public:
	T data;
	Node* precessor = NULL;
	Node* successor = NULL;
	Node* sibling = NULL;
	Node(T& _data, Key _key) {
		data = _data;
		key = _key;
	}
	Node(T& _data, Key _key, Node* parent) {
		data = _data;
		precessor = parent;
		key = _key;
	}
	Key key;
};

template<typename T, typename Key>
class BinaryTree {
public:
	Node<T, Key>* root = NULL;
	BinaryTree(string _name) {
		name = _name;
	}
	BinaryTree(T& data, Key key) {
		
		root = new Node<T, Key>(data, key);
	}
	int count() {
		return countOfNodes;
	}
	/**
	루트가 비어있으면 루트에 넣습니다. 루트가 차 있으면 가장 트리의 가장 왼쪽 아래에 넣습니다.
	**/
	void insert_back(T& data, Key key) {
		if (root == NULL)
			root = new Node<T, Key>(data, key);
		else
		{
			Node<T, Key>* temp = root;
			while (temp->successor != NULL)
				temp = temp->successor;
			temp->successor = new Node<T, Key>(data, key, temp);
		}
		countOfNodes++;
	}
	void insert_back(Node<T, Key>* node) {
		if (root == NULL)
			root = node;
		else
		{
			Node<T, Key>* temp = root;
			while (temp->successor != NULL)
				temp = temp->successor;
			temp->successor = node;
			node->precessor = temp;
		}
		countOfNodes += 1;
	}
	void insert_back(BinaryTree* tree2) {
		if (root == NULL)
			root = tree2->root;
		else
		{
			Node<T, Key>* temp = root;
			while (temp->successor != NULL)
				temp = temp->successor;
			temp->successor = tree2->root;
			tree2->root->precessor = temp;
		}
		countOfNodes += tree2->count();
	}
	void insertAsChildren(T& data, Key dataKey, Key parentKey) {
		Node<T, Key>* temp = Find(parentKey);
		if (temp != NULL)
		{
			Node<T, Key>* temp2 = new Node<T, Key>(data, dataKey, temp);
			temp->successor = temp2;
			countOfNodes++;
		}
	}
	void insertAsChildren(BinaryTree* tree2, Key parentKey) {
		Node<T, Key>* temp = Find(parentKey);
		if (temp != NULL)
		{
			temp->successor = tree2->root;
			tree2->root->precessor = temp;
			countOfNodes += tree2->count();
		}
	}
	void insertAsSibling(T& data, Key dataKey, Key siblingKey) {
		Node<T, Key>* temp = Find(siblingKey);
		if (temp == NULL)
			return;
		while (temp->sibling != NULL)
		{
			temp = temp->sibling;
		}
		Node<T, Key>* temp2 = new Node<T, Key>(data, dataKey, temp);
		temp->sibling = temp2;
		countOfNodes++;
	}
	void insertAsSibling(BinaryTree* tree2, Key siblingKey) {
		Node<T, Key>* temp = Find(siblingKey);
		if (temp == NULL)
			return;
		while (temp->sibling != NULL)
		{
			temp = temp->sibling;
		}

		temp->successor = tree2->root;
		tree2->root->precessor = temp;
		countOfNodes += tree2->count();
	}
	Node<T, Key>* Find(Key key) {
		return Find_aux(key, root);
	}

	void clear() {
		clear_aux(root);
		countOfNodes = 0;
	}
	
	string name;
private:
	int countOfNodes = 0;
	Node<T, Key>* Find_aux(Key key, Node<T, Key>* root) {
		if (root == NULL)
			return NULL;
		if (root->key == key)
			return root;
		else
		{
			Node<T, Key>* temp = Find_aux(key, root->successor);
			if (temp != NULL)
				return temp;
			else
				return Find_aux(name, root->successor);
		}
	}
	void clear_aux(Node<T, Key>* root) {
		if (root == NULL)
			return;
		clear_aux(root->sibling);
		clear_aux(root->successor);
		delete root;
	}
};