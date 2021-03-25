#ifndef VM_H
#define VM_H

#include "main.h"

bool checkSpace(string str);

bool checkRegister(string str);

bool checkInteger(string str);

bool checkFloat(string str);

bool checkBoolean(string str);

bool checkAddress(string str);

bool checkLiteral(string str);

string checkType(string str);

class Address
{
private:
  int i;

public:
  Address(int i = 0);

  friend ostream &operator<<(ostream &os, const Address &a);

  int getInt();
  void setInt(int i);
};

enum Type
{
  INTEGER,
  FLOAT,
  BOOL,
  ADDRESS,
  UNDEFINED
};

class MyType
{
private:
  Type type;
  int i;
  float f;
  bool b;
  Address a;

public:
  MyType();

  MyType(int i);

  MyType(float f);

  MyType(bool b);

  MyType(Address a);

  MyType(string s);

  friend ostream &operator<<(ostream &os, const MyType &m);

  void setValue(int i);

  void setValue(float f);

  void setValue(bool b);

  void setValue(Address a);

  Type getType();

  int getInt();

  float getFloat();

  bool getBool();

  Address getAddress();

  MyType operator+(MyType &m);
  
  MyType operator-(MyType &m);

  MyType operator*(MyType &m);

  MyType operator/(MyType &m);

  MyType operator==(MyType &m);

  MyType operator!=(MyType &m);

  MyType operator<(MyType &m);

  MyType operator<=(MyType &m);

  MyType operator>(MyType &m);

  MyType operator>=(MyType &m);

  MyType operator!();

  MyType operator&&(MyType &m);

  MyType operator||(MyType &m);
};

class Register
{
private:
	MyType val;

public:
	Register();

	Register(MyType m);

	Register(string s);

	friend ostream &operator<<(ostream &os, const Register &r);

	friend istream &operator>>(istream &is, Register &r);

	MyType getVal();

	void setVal(MyType val);

	Register operator+(Register &r);

  Register operator-(Register &r);

  Register operator*(Register &r);

  Register operator/(Register &r);

  Register operator==(Register &r);

  Register operator!=(Register &r);

  Register operator<(Register &r);

  Register operator<=(Register &r);

  Register operator>(Register &r);

  Register operator>=(Register &r);

  Register operator!();

  Register operator&&(Register &r);

  Register operator||(Register &r);

};

template <class T>
class DLinkedList
{
public:
	class Node;			// Forward declaration
	class Iterator; //forward declaration
protected:
	Node *head;
	Node *tail;
	int count;

public:
	DLinkedList() : head(NULL), tail(NULL), count(0){};
	~DLinkedList(){};
	void add(const T &e);
	void add(int index, const T &e);
	T removeAt(int index);
	bool removeItem(const T &item);
	void clear();
	int size();
	bool empty();
	T get(int index);
	void set(int index, const T &e);
	int indexOf(const T &item);
	bool contains(const T &item);
	Iterator begin()
	{
		return Iterator(this, true);
	}
	Iterator end()
	{
		return Iterator(this, false);
	}

public:
	class Node
	{
	private:
		T data;
		Node *next;
		Node *previous;
		friend class DLinkedList<T>;

	public:
		Node()
		{
			this->previous = NULL;
			this->next = NULL;
		}

		Node(const T &data)
		{
			this->data = data;
			this->previous = NULL;
			this->next = NULL;
		}
	};
	class Iterator
	{
	private:
		DLinkedList<T> *pList;
		Node *current;
		int index; // is the index of current in pList
	public:
		Iterator(DLinkedList<T> *pList, bool begin);
		Iterator &operator=(const Iterator &iterator);
		void set(const T &e);
		T &operator*();
		bool operator!=(const Iterator &iterator);
		void remove();

		// Prefix ++ overload
		Iterator &operator++();

		// Postfix ++ overload
		Iterator operator++(int);
	};
};

// Stack

template <class T>
class Stack
{
protected:
	DLinkedList<T> list;

public:
	Stack() {}
	void push(T item);
	T pop();
	T top();
	bool empty();
	int size();
	void clear();
};

class VM
{
public:
  VM() {}
  void run(string filename);
};
#endif