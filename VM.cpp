#include "VM.h"

string o = "0A";
Register IP = Register(o);

bool checkSpace(string str)
{
	if (str.substr(0, 1) == " " || str.substr(str.length() - 1) == " ")
		return true;
	return false;
}

bool checkRegister(string str)
{
	std::regex registerRegex("^R([1-9]|1[0-5])$");
	if (regex_match(str, registerRegex))
		return true;
	return false;
}

bool checkInteger(string str)
{
	std::regex integer("[[:digit:]]+");
	if (regex_match(str, integer))
		return true;
	return false;
}

bool checkFloat(string str)
{
	std::regex floatRegex("(([0-9]\\d*\\.)([0-9]\\d*))"); // chua loai duoc TH -000.000

	if (regex_match(str, floatRegex))
		return true;
	return false;
}

bool checkBoolean(string str)
{
	if (str == "true" || str == "false")
		return true;
	return false;
}

bool checkAddress(string str)
{
	std::regex integer("[[:digit:]]+A$");
	if (regex_match(str, integer))
		return true;
	return false;
}

bool checkLiteral(string str)
{
	if (checkInteger(str) || checkFloat(str) || checkBoolean(str) || checkAddress(str))
		return true;
	return false;
}

string checkType(string str)
{
	if (checkInteger(str))
		return "integer";
	if (checkFloat(str))
		return "float";
	if (checkBoolean(str))
		return "boolean";
	if (checkAddress(str))
		return "address";
	return "undefined";
}

// Address

Address::Address(int i)
{
	this->i = i;
}

ostream &operator<<(ostream &os, const Address &a)
{
	cout << a.i << 'A';
	return os;
}

int Address::getInt()
{
	return this->i;
}

void Address::setInt(int i)
{
	this->i = i;
}

// MyType

MyType::MyType()
{
	this->type = Type::UNDEFINED;
}

MyType::MyType(int i)
{
	this->i = i;
	this->type = Type::INTEGER;
}

MyType::MyType(float f)
{
	this->f = f;
	this->type = Type::FLOAT;
}

MyType::MyType(bool b)
{
	this->b = b;
	this->type = Type::BOOL;
}

MyType::MyType(Address a)
{
	this->a = a;
	this->type = Type::ADDRESS;
}

MyType::MyType(string s)
{
	string type = checkType(s);
	if (type == "integer")
	{
		setValue(stoi(s));
		// this->setValue(stoi(s));
	}
	else if (type == "address")
	{
		setValue(Address(stoi(s.substr(0, s.length() - 1))));
	}
	else if (type == "float")
	{
		setValue(stof(s));
	}
	else if (type == "boolean")
	{

		setValue((s == "true") ? true : false);
	}
}

ostream &operator<<(ostream &os, const MyType &m)
{
	// std::regex floatOut("^[[:digit:]]+\\.0+$");
	switch (m.type)
	{
	case Type::INTEGER:
		os << m.i;
		break;
	case Type::FLOAT:
		// if (regex_match(to_string(m.f), floatOut))
		// {
		// 	os << m.f << ".0";
		// }
		// else
		// {
		// 	os << m.f;
		// };
		os << m.f;
		break;
	case Type::BOOL:
		if (m.b) {
			os << "true";
		}
		else
		{
			os << "false";
		} 
		break;
	case Type::ADDRESS:
		os << m.a;
		break;
	default:
		os << "UNDEFIEND"; // or throw exception
		break;
	}
	return os;
}

void MyType::setValue(int i)
{
	this->i = i;
	this->type = INTEGER;
}

void MyType::setValue(float f)
{
	this->f = f;
	this->type = FLOAT;
}

void MyType::setValue(bool b)
{
	this->b = b;
	this->type = BOOL;
}

void MyType::setValue(Address a)
{
	this->a = a;
	this->type = ADDRESS;
}

Type MyType::getType()
{
	return this->type;
}

int MyType::getInt()
{
	return this->i;
}

float MyType::getFloat()
{
	return this->f;
}

bool MyType::getBool()
{
	return this->b;
}

Address MyType::getAddress()
{
	return this->a;
}

MyType MyType::operator+(MyType &m)
{
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		int temp = this->getInt() + m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		float temp = this->getFloat() + m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		float temp = this->getInt() + m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		float temp = this->getFloat() + m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator-(MyType &m)
{
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		int temp = this->getInt() - m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		float temp = this->getFloat() - m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		float temp = this->getInt() - m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		float temp = this->getFloat() - m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator*(MyType &m)
{
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		int temp = this->getInt() * m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		float temp = this->getFloat() * m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		float temp = this->getInt() * m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		float temp = this->getFloat() * m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator/(MyType &m)
{
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		if (m.getInt() == 0)
			throw DivideByZero(IP.getVal().getAddress().getInt() - 1);
		int temp = this->getInt() / m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		if (m.getFloat() == 0)
			throw DivideByZero(IP.getVal().getAddress().getInt() - 1);
		float temp = this->getFloat() / m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		if (m.getFloat() == 0)
			throw DivideByZero(IP.getVal().getAddress().getInt() - 1);
		float temp = this->getInt() / m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		if (m.getInt() == 0)
			throw DivideByZero(IP.getVal().getAddress().getInt() - 1);
		float temp = this->getFloat() / m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator==(MyType &m)
{
	if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	{
		bool temp = this->getBool() == m.getBool();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		bool temp = this->getInt() == m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		bool temp = this->getFloat() == m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		bool temp = this->getInt() == m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		bool temp = this->getFloat() == m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator!=(MyType &m)
{
	if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	{
		bool temp = this->getBool() != m.getBool();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		bool temp = this->getInt() != m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		bool temp = this->getFloat() != m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		bool temp = this->getInt() != m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		bool temp = this->getFloat() != m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator<(MyType &m)
{
	// if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	// {
	// 	bool temp = this->getBool() < m.getBool();
	// 	MyType type(temp);
	// 	return type;
	// }
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		bool temp = this->getInt() < m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		bool temp = this->getFloat() < m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		bool temp = this->getInt() < m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		bool temp = this->getFloat() < m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator<=(MyType &m)
{
	// if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	// {
	// 	bool temp = this->getBool() <= m.getBool();
	// 	MyType type(temp);
	// 	return type;
	// }
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		bool temp = this->getInt() <= m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		bool temp = this->getFloat() <= m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		bool temp = this->getInt() <= m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		bool temp = this->getFloat() <= m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator>(MyType &m)
{
	// if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	// {
	// 	bool temp = this->getBool() > m.getBool();
	// 	MyType type(temp);
	// 	return type;
	// }
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		bool temp = this->getInt() > m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		bool temp = this->getFloat() > m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		bool temp = this->getInt() > m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		bool temp = this->getFloat() > m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator>=(MyType &m)
{
	// if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	// {
	// 	bool temp = this->getBool() >= m.getBool();
	// 	MyType type(temp);
	// 	return type;
	// }
	if (this->type == Type::INTEGER && m.getType() == Type::INTEGER)
	{
		bool temp = this->getInt() >= m.getInt();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::FLOAT)
	{
		bool temp = this->getFloat() >= m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::INTEGER && m.getType() == Type::FLOAT)
	{
		bool temp = this->getInt() >= m.getFloat();
		MyType type(temp);
		return type;
	}
	else if (this->type == Type::FLOAT && m.getType() == Type::INTEGER)
	{
		bool temp = this->getFloat() >= m.getInt();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator!()
{
	if (this->type == Type::BOOL)
	{
		bool temp = !this->getBool();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator&&(MyType &m)
{
	if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	{
		bool temp = this->getBool() && m.getBool();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

MyType MyType::operator||(MyType &m)
{
	if (this->type == Type::BOOL && m.getType() == Type::BOOL)
	{
		bool temp = this->getBool() || m.getBool();
		MyType type(temp);
		return type;
	}
	else
		throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
	return MyType();
}

// Register

Register::Register() {}

Register::Register(MyType m)
{
	this->val = m;
}

Register::Register(string s)
{
	string type = checkType(s);
	if (type == "integer")
	{
		this->val = MyType(stoi(s));
		// this->setValue(stoi(s));
	}
	else if (type == "address")
	{
		this->val = MyType(Address(stoi(s.substr(0, s.length() - 1))));
	}
	else if (type == "float")
	{
		this->val = MyType(stof(s));
	}
	else if (type == "boolean")
	{
		this->val = MyType((s == "true") ? true : false);
	}
}

ostream &operator<<(ostream &os, const Register &r)
{
	os << r.val;
	return os;
}

istream &operator>>(istream &is, Register &r)
{
	string input;
	// is >> input;
	getline(is, input);
	r = Register(input);
	if (r.getVal().getType() == Type::ADDRESS || r.getVal().getType() == Type::UNDEFINED)
		throw InvalidInput(IP.getVal().getAddress().getInt());
	return is;
}

MyType Register::getVal()
{
	return this->val;
}

void Register::setVal(MyType val)
{
	this->val = val;
}

Register Register::operator+(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 + m2);
}

Register Register::operator-(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 - m2);
}

Register Register::operator*(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 * m2);
}

Register Register::operator/(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 / m2);
}

Register Register::operator==(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 == m2);
}

Register Register::operator!=(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 != m2);
}

Register Register::operator<(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 < m2);
}

Register Register::operator<=(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 <= m2);
}

Register Register::operator>(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 > m2);
}

Register Register::operator>=(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 >= m2);
}

Register Register::operator!()
{
	MyType m1 = this->getVal();
	return Register(!m1);
}

Register Register::operator&&(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 && m2);
}

Register Register::operator||(Register &r)
{
	MyType m1 = this->getVal();
	MyType m2 = r.getVal();
	return Register(m1 || m2);
}

// LinkedList

template <class T>
void DLinkedList<T>::add(const T &e)
{
	/* Insert an element into the end of the list. */
	Node *newNode = new Node(e);
	if (this->count == 0)
	{
		this->head = newNode;
		this->tail = newNode;
	}
	else
	{
		this->tail->next = newNode;
		newNode->previous = this->tail;
		this->tail = newNode;
	}
	this->count++;
}

template <class T>
void DLinkedList<T>::add(int index, const T &e)
{
	/* Insert an element into the list at given index. */
	if (index < 0 || index > this->count)
	{
		throw std::out_of_range("The index is out of range!");
	}

	if (index == this->count)
	{
		this->add(e);
		return;
	}

	Node *newNode = new Node(e);
	if (index == 0)
	{
		newNode->next = this->head;
		this->head->previous = newNode;
		this->head = newNode;
	}
	else
	{
		Node *prevNewNode = nullptr;
		Node *nextNewNode = nullptr;
		if (index < this->count / 2)
		{
			prevNewNode = this->head;
			int i = 0;
			while (prevNewNode != nullptr)
			{
				if (i == index - 1)
				{
					break;
				}
				prevNewNode = prevNewNode->next;
				i++;
			}
			nextNewNode = prevNewNode->next;
		}
		else
		{
			nextNewNode = this->tail;
			int i = this->count - 1;
			while (nextNewNode != nullptr)
			{
				if (i == index)
				{
					break;
				}
				nextNewNode = nextNewNode->previous;
				i--;
			}
			prevNewNode = nextNewNode->previous;
		}
		prevNewNode->next = newNode;
		newNode->previous = prevNewNode;
		newNode->next = nextNewNode;
		nextNewNode->previous = newNode;
	}
	this->count++;
}

template <class T>
T DLinkedList<T>::removeAt(int index)
{
	/* Remove element at index and return removed value */
	if (index < 0 || index >= this->count)
	{
		throw std::out_of_range("The index is out of range!");
	}

	T val = this->head->data;
	if (this->count == 1)
	{
		Node *delNode = this->head;
		val = delNode->data;
		this->head = nullptr;
		this->tail = nullptr;
		delete delNode;
	}
	else
	{
		if (index == 0)
		{
			Node *delNode = this->head;
			val = delNode->data;
			Node *nextDelNode = delNode->next;
			delNode->next = nullptr;
			nextDelNode->previous = nullptr;
			this->head = nextDelNode;
			delete delNode;
		}
		else if (index == this->count - 1)
		{
			Node *delNode = this->tail;
			val = delNode->data;
			Node *prevDelNode = delNode->previous;
			prevDelNode->next = nullptr;
			delNode->previous = nullptr;
			this->tail = prevDelNode;
			delete delNode;
		}
		else
		{
			Node *delNode = nullptr;
			Node *prevDelNode = nullptr;
			Node *nextDelNode = nullptr;
			if (index < this->count / 2)
			{
				delNode = this->head;
				int i = 0;
				while (delNode != nullptr)
				{
					if (i == index)
					{
						break;
					}
					delNode = delNode->next;
					i++;
				}
			}
			else
			{
				delNode = this->tail;
				int i = this->count - 1;
				while (delNode != nullptr)
				{
					if (i == index)
					{
						break;
					}
					delNode = delNode->previous;
					i--;
				}
			}
			val = delNode->data;
			prevDelNode = delNode->previous;
			nextDelNode = delNode->next;

			delNode->next = nullptr;
			delNode->previous = nullptr;
			prevDelNode->next = nextDelNode;
			nextDelNode->previous = prevDelNode;
			delete delNode;
		}
	}
	this->count--;
	return val;
}

template <class T>
bool DLinkedList<T>::removeItem(const T &item)
{
	/* Remove the first apperance of item in list and return true, otherwise return false */
	Node *delNode = this->head;
	int index = 0;
	while (delNode != nullptr)
	{
		if (delNode->data == item)
		{
			this->removeAt(index);
			return true;
		}
		delNode = delNode->next;
		index++;
	}
	return false;
}

template <class T>
void DLinkedList<T>::clear()
{
	/* Remove all elements in list */
	while (this->count != 0)
	{
		this->removeAt(0);
	}
}

template <class T>
int DLinkedList<T>::size()
{
	/* Return the length (size) of list */
	return this->count;
}

template <class T>
T DLinkedList<T>::get(int index)
{
	/* Give the data of the element at given index in the list. */
	if (index < 0 || index >= this->count)
	{
		throw std::out_of_range("The index is out of range!");
	}

	Node *getNode = nullptr;
	if (index < this->count / 2)
	{
		getNode = this->head;
		int i = 0;
		while (getNode != nullptr)
		{
			if (i == index)
			{
				break;
			}
			getNode = getNode->next;
			i++;
		}
	}
	else
	{
		getNode = this->tail;
		int i = this->count - 1;
		while (getNode != nullptr)
		{
			if (i == index)
			{
				break;
			}
			getNode = getNode->previous;
			i--;
		}
	}
	return getNode->data;
}

template <class T>
void DLinkedList<T>::set(int index, const T &e)
{
	/* Assign new value for element at given index in the list */
	if (index < 0 || index >= this->count)
	{
		throw std::out_of_range("The index is out of range!");
	}

	Node *setNode = nullptr;
	if (index < this->count / 2)
	{
		setNode = this->head;
		int i = 0;
		while (setNode != nullptr)
		{
			if (i == index)
			{
				setNode->data = e;
				return;
			}
			setNode = setNode->next;
			i++;
		}
	}
	else
	{
		setNode = this->tail;
		int i = this->count - 1;
		while (setNode != nullptr)
		{
			if (i == index)
			{
				setNode->data = e;
				return;
			}
			setNode = setNode->previous;
			i--;
		}
	}
}

template <class T>
bool DLinkedList<T>::empty()
{
	/* Check if the list is empty or not. */
	return this->count == 0;
}

template <class T>
int DLinkedList<T>::indexOf(const T &item)
{
	/* Return the first index wheter item appears in list, otherwise return -1 */
	Node *getNode = this->head;
	int index = 0;
	while (getNode != nullptr)
	{
		if (getNode->data == item)
		{
			return index;
		}
		getNode = getNode->next;
		index++;
	}
	return -1;
}

template <class T>
bool DLinkedList<T>::contains(const T &item)
{
	/* Check if item appears in the list */
	if (this->indexOf(item) != -1)
	{
		return true;
	}
	return false;
}

template <class T>
DLinkedList<T>::Iterator::Iterator(DLinkedList<T> *pList, bool begin)
{
	this->pList = pList;
	if (begin)
	{
		if (this->pList != nullptr)
		{
			this->current = this->pList->head;
			this->index = 0;
		}
		else
		{
			this->current = nullptr;
			this->index = -1;
		}
	}
	else
	{
		this->current = nullptr;
		if (this->pList != nullptr)
		{
			this->index = this->pList->count;
		}
		else
		{
			this->index = 0;
		}
	}
}

template <class T>
typename DLinkedList<T>::Iterator &DLinkedList<T>::Iterator::operator=(const DLinkedList<T>::Iterator &iterator)
{
	this->pList = iterator.pList;
	this->current = iterator.current;
	this->index = iterator.index;
	return *this;
}

template <class T>
void DLinkedList<T>::Iterator::set(const T &e)
{
	if (this->current == nullptr)
	{
		throw std::out_of_range("Segmentation fault!");
	}

	this->current->data = e;
}

template <class T>
T &DLinkedList<T>::Iterator::operator*()
{
	if (this->current == nullptr)
	{
		throw std::out_of_range("Segmentation fault!");
	}

	return this->current->data;
}

template <class T>
void DLinkedList<T>::Iterator::remove()
{
	/*
    * TODO: delete Node in pList which Node* current point to. 
    *       After that, Node* current point to the node before the node just deleted.
    *       If we remove first node of pList, Node* current point to nullptr.
    *       Then we use operator ++, Node* current will point to the head of pList.
    */
	if (this->current == nullptr)
	{
		throw std::out_of_range("Segmentation fault!");
	}

	Node *prevDelNode = this->current->previous;
	this->pList->removeItem(this->current->data);
	this->current = prevDelNode;
	this->index--;
}

template <class T>
bool DLinkedList<T>::Iterator::operator!=(const DLinkedList::Iterator &iterator)
{
	return this->current != iterator.current || this->index != iterator.index;
}

template <class T>
typename DLinkedList<T>::Iterator &DLinkedList<T>::Iterator::operator++()
{
	if (this->current == nullptr)
	{
		if (this->index == -1)
		{
			this->current = this->pList->head;
		}
		else
		{
			throw std::out_of_range("Segmentation fault!");
		}
	}
	else
	{
		this->current = this->current->next;
	}
	this->index++;
	return *this;
}

template <class T>
typename DLinkedList<T>::Iterator DLinkedList<T>::Iterator::operator++(int)
{
	Iterator temp = *this;
	if (this->current == nullptr)
	{
		if (this->index == -1)
		{
			this->current = this->pList->head;
		}
		else
		{
			throw std::out_of_range("Segmentation fault!");
		}
	}
	else
	{
		this->current = this->current->next;
	}
	this->index++;
	return temp;
}

// stack

template <class T>
void Stack<T>::push(T item)
{
	// TODO: Push new element into the top of the stack
	if (this->list.size() >= 1000)
	{
		throw StackFull(IP.getVal().getAddress().getInt() - 1);
	}
	this->list.add(0, item);
}

template <class T>
T Stack<T>::pop()
{
	// TODO: Remove an element on top of the stack
	T val;
	if (this->empty() != true)
	{
		val = this->list.removeAt(0);
	}
	return val;
}

template <class T>
T Stack<T>::top()
{
	// TODO: Get value of the element on top of the stack
	return this->list.get(0);
}

template <class T>
bool Stack<T>::empty()
{
	// TODO: Determine if the stack is empty
	return this->list.size() == 0;
}

template <class T>
int Stack<T>::size()
{
	// TODO: Get the size of the stack
	return this->list.size();
}

template <class T>
void Stack<T>::clear()
{
	// TODO: Clear all elements of the stack
	while (this->list.size() != 0)
	{
		this->list.removeAt(0);
	}
}

void VM::run(string filename)
{
	Stack<Address> stack;
	Register registers[15] = {};
	MyType *memory = new MyType[UINT16_MAX]{};
	string *cmdArr = new string[UINT16_MAX]{};

	ifstream inputRe(filename);
	if (inputRe.fail())
	{
		std::cout << "Failed to open this file!" << std::endl;
	}

	int check = 0;
	while (!inputRe.eof())
	{
		char line[255];
		inputRe.getline(line, 255);
		cmdArr[check] = line;

		// kiem tra Lỗi lúc nạp chương trình

		// kiem tra co khoang trang dau va cuoi dong lenh khong
		if (checkSpace(cmdArr[check]))
		{
			throw InvalidInstruction(IP.getVal().getAddress().getInt());
		};

		// xu ly cac thanh phan cua moi dong lenh:
		// cau truc lenh
		// 1: command register, register/literal
		// 2: command register/literal
		// 3: command

		string cmdType = "", firstVar = "", secondVar = "";

		if (cmdArr[check].find(' ') != string::npos)
		{ // neu co ton tai " " => TH1: co toan hang, TH2: sai cu phap
			cmdType = cmdArr[check].substr(0, cmdArr[check].find(' '));
			if (cmdArr[check].find(", ") != string::npos)
			{ // ", " co ton tai => TH1: co toan hang thu 2 => toan hang 1 bat buoc la thanh ghi, TH2: sai cu phap
				if (cmdArr[check].substr(cmdArr[check].find(' ') + 1, 1) != "R")
				{
					firstVar = cmdArr[check].substr(cmdArr[check].find(' ') + 1, cmdArr[check].find(',') - cmdArr[check].find(' ') - 1);
				}
				else
				{
					int temp = cmdArr[check].find(',') - cmdArr[check].find(' ');
					if (temp == 3)
					{
						firstVar = cmdArr[check].substr(cmdArr[check].find(' ') + 1, 2);
					}
					else
					{
						firstVar = cmdArr[check].substr(cmdArr[check].find(' ') + 1, 3);
					}
				}

				if (cmdArr[check].substr(cmdArr[check].find(", ") + 2, 1) == "R")
				{
					int temp2 = cmdArr[check].substr(cmdArr[check].find(", ") + 2).length();
					if (temp2 == 2)
					{
						secondVar = cmdArr[check].substr(cmdArr[check].find(", ") + 2, 2);
					}
					else if (temp2 == 3)
					{
						secondVar = cmdArr[check].substr(cmdArr[check].find(", ") + 2, 3);
					}
				}
				else
				{
					secondVar = cmdArr[check].substr(cmdArr[check].find(", ") + 2);
				}
			}
			else
			{
				if (cmdArr[check].substr(cmdArr[check].find(' ') + 1, 1) == "R")
				{
					firstVar = cmdArr[check].substr(cmdArr[check].find(' ') + 1);
				}
				else
				{
					firstVar = cmdArr[check].substr(cmdArr[check].find(' ') + 1);
				}
			}
		}
		else
		{
			cmdType = cmdArr[check].substr(0);
		}

		if (cmdType == "Add" || cmdType == "Minus" || cmdType == "Mul" || cmdType == "Div" || cmdType == "CmpEQ" || cmdType == "CmpNE" || cmdType == "CmpLT" || cmdType == "CmpLE" || cmdType == "CmpGT" || cmdType == "CmpGE" || cmdType == "And" || cmdType == "Or" || cmdType == "Move" || cmdType == "Load" || cmdType == "Store" || cmdType == "JumpIf")
		{ // co 2 toan hang
			if (firstVar == "" || secondVar == "")
			{
				throw InvalidInstruction(check);
			}
			else if (secondVar.find(", ") != string::npos) // bien thu hai co chuoi ", " => co them mot toan hang?!
			{
				throw InvalidInstruction(check);
			}
			else if (!checkRegister(firstVar))
			{
				throw InvalidOperand(check);
			}
			else if (!checkRegister(secondVar) && !checkLiteral(secondVar))
			{
				throw InvalidOperand(check);
			}
		}
		else if (cmdType == "Not" || cmdType == "Input")
		{ //1 toan hang : dest
			if (firstVar == "" || secondVar != "")
			{
				throw InvalidInstruction(check);
			}
			else if (!checkRegister(firstVar))
			{
				throw InvalidOperand(check);
			}
		}
		else if (cmdType == "Jump" || cmdType == "Call" || cmdType == "Output")
		{ //1 toan hang : src
			if (firstVar == "" || secondVar != "")
			{
				throw InvalidInstruction(check);
			}
			else if (!checkRegister(firstVar) && !checkLiteral(firstVar))
			{
				throw InvalidOperand(check);
			}
		}
		else if (cmdType == "Return" || cmdType == "Halt")
		{ //khong co toan hang
			if (cmdArr[check].length() != cmdType.length())
				throw InvalidInstruction(check);
		}
		else
		{
			throw InvalidInstruction(check);
		}

		check++;
	}

	inputRe.close();

	int currentIP = 0;
	while (currentIP < UINT16_MAX)
	{

		// Đọc lệnh tại địa chỉ hiện tại
		string cmdType = "", firstVar = "", secondVar = "";
		int seqR = 0, seqR2 = 0;

		if (cmdArr[currentIP].find(' ') != string::npos)
		{ // neu co ton tai " " => TH1: co toan hang, TH2: sai cu phap
			cmdType = cmdArr[currentIP].substr(0, cmdArr[currentIP].find(' '));
			if (cmdArr[currentIP].find(", ") != string::npos)
			{ // ", " co ton tai => TH1: co toan hang thu 2 => toan hang 1 bat buoc la thanh ghi, TH2: sai cu phap
				if (cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1, 1) != "R")
				{
					// throw exception do firstVar ko phai Thanh ghi
					firstVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1, cmdArr[currentIP].find(',') - cmdArr[currentIP].find(' ') - 1);
				}
				else
				{
					int temp = cmdArr[currentIP].find(',') - cmdArr[currentIP].find(' ');
					if (temp == 3)
					{
						firstVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1, 2);
						seqR = stoi(firstVar.substr(1, 1));
					}
					else
					{
						firstVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1, 3);
						seqR = stoi(firstVar.substr(1, 2));
					}
				}

				if (cmdArr[currentIP].substr(cmdArr[currentIP].find(", ") + 2, 1) == "R")
				{
					int temp2 = cmdArr[currentIP].substr(cmdArr[currentIP].find(", ") + 2).length();
					if (temp2 == 2)
					{
						secondVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(", ") + 2, 2);
						seqR2 = std::stoi(secondVar.substr(1, 1));
					}
					else if (temp2 == 3)
					{
						secondVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(", ") + 2, 3);
						seqR2 = std::stoi(secondVar.substr(1, 2));
					}
				}
				else
				{
					secondVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(", ") + 2);
				}
			}
			else
			{
				if (cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1, 1) == "R")
				{
					firstVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1);
					seqR = stoi(firstVar.substr(1));
				}
				else
				{
					firstVar = cmdArr[currentIP].substr(cmdArr[currentIP].find(' ') + 1);
				}
			}
		}
		else
		{
			cmdType = cmdArr[currentIP].substr(0);
		}
		// cout << "\ncmdtype: " << cmdType << " firstVar: " << firstVar << " secondVar: " << secondVar << " seqR: " << seqR << " seqR2: " << seqR2 << "\n";

		// Tăng địa chỉ trong thanh ghi IP lên 1.
		currentIP++;
		// cap nhat lai dia chi duoc luu tai IP
		string newStr = to_string(currentIP) + "A";
		MyType newIP(newStr);
		IP.setVal(newIP);

		// xử lý lệnh

		// Lệnh điều khiển trình tự
		if (cmdType == "Halt")
		{
			break;
		}

		// Các lệnh đọc và ghi dữ liệu
		if (cmdType == "Move")
		{
			registers[seqR - 1] = Register(secondVar);
		}

		if (cmdType == "Store")
		{

			MyType val = registers[seqR - 1].getVal();
			Type type = val.getType();
			if (type != Type::ADDRESS)
			{
				throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
				break;
			}

			Address addr = val.getAddress();

			if (secondVar.substr(0, 1) == "R")
			{
				memory[addr.getInt()] = registers[seqR2 - 1].getVal();
			}
			else
			{
				memory[addr.getInt()] = MyType(secondVar);
			}

			// cout << "Register: " << registers[seqR - 1].getVal().getAddress().getInt()
			// 	<< " Memory: " << memory[addr.getInt()].getInt() << '\n';
		}

		if (cmdType == "Load")
		{
			MyType val = registers[seqR2 - 1].getVal();
			Type type = val.getType();
			if (type != Type::ADDRESS)
			{
				throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
				break;
			}
			Address addr = val.getAddress();

			registers[seqR - 1].setVal(memory[addr.getInt()]);
		}

		//  Các lệnh nhập xuất dữ liệu
		if (cmdType == "Input")
		{
			cin >> registers[seqR - 1];
		}

		if (cmdType == "Output")
		{
			if (firstVar.substr(0, 1) == "R")
			{
				cout << registers[seqR - 1];
			}
			else
			{
				Register tmp(firstVar);
				cout << tmp;
			}
		}

		// 	Các lệnh số học

		if (cmdType == "Add")
		{
			// kieerm tra kieu du lieu phai la int hoac float
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] + registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] + tmp;
			}
		}

		if (cmdType == "Minus")
		{
			// kieerm tra kieu du lieu phai la int hoac float
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] - registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] - tmp;
			}
		}

		if (cmdType == "Mul")
		{
			// kieerm tra kieu du lieu phai la int hoac float
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] * registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] * tmp;
			}
		}

		if (cmdType == "Div")
		{
			// kieerm tra kieu du lieu phai la int hoac float
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] / registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] / tmp;
			}
		}

		// Các lệnh so sánh
		if (cmdType == "CmpEQ")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] == registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] == tmp;
			}
		}

		if (cmdType == "CmpNE")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] != registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] != tmp;
			}
		}

		if (cmdType == "CmpLT")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] < registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] < tmp;
			}
		}

		if (cmdType == "CmpLE")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] <= registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] <= tmp;
			}
		}

		if (cmdType == "CmpGT")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] > registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] > tmp;
			}
		}

		if (cmdType == "CmpGE")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] >= registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] >= tmp;
			}
		}

		// Các lệnh luận lý
		if (cmdType == "Not")
		{
			registers[seqR - 1] = !registers[seqR - 1];
		}

		if (cmdType == "And")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] && registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] && tmp;
			}
		}

		if (cmdType == "Or")
		{
			if (secondVar.substr(0, 1) == "R")
			{
				registers[seqR - 1] = registers[seqR - 1] || registers[seqR2 - 1];
			}
			else
			{
				Register tmp(secondVar);
				registers[seqR - 1] = registers[seqR - 1] || tmp;
			}
		}

		if (cmdType == "Jump")
		{
			if (checkAddress(firstVar))
			{
				MyType jumpIP(firstVar);
				if (jumpIP.getAddress().getInt() < check)
				{
					IP.setVal(jumpIP);
					currentIP = IP.getVal().getAddress().getInt();
				}
				else
					throw InvalidDestination(IP.getVal().getAddress().getInt() - 1);
			}
			else
				throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
		}

		if (cmdType == "JumpIf")
		{
			if (registers[seqR - 1].getVal().getType() == Type::BOOL)
			{
				if (registers[seqR - 1].getVal().getBool() == true)
				{
					if (checkAddress(secondVar))
					{

						MyType jumpIP(secondVar);
						if (jumpIP.getAddress().getInt() < check)
						{
							IP.setVal(jumpIP);
							currentIP = IP.getVal().getAddress().getInt();
						}
						else
							throw InvalidDestination(IP.getVal().getAddress().getInt() - 1);
					}
					else
						throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
				}
			}
			else
				throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
		}

		if (cmdType == "Call")
		{

			if (firstVar.substr(0, 1) == "R")
			{
				if (registers[seqR - 1].getVal().getType() == Type::ADDRESS)
				{
					if (registers[seqR - 1].getVal().getAddress().getInt() < check)
					{
						Address nextCmdAddr(IP.getVal().getAddress().getInt());
						stack.push(nextCmdAddr);
						IP.setVal(registers[seqR - 1].getVal());
					}
					else
						throw InvalidDestination(IP.getVal().getAddress().getInt() - 1);
				}
				else
					throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
			}
			else
			{
				if (checkAddress(firstVar))
				{
					MyType jumpIP(firstVar);
					if (jumpIP.getAddress().getInt() < check)
					{
						Address nextCmdAddr(IP.getVal().getAddress().getInt());
						stack.push(nextCmdAddr);
						IP.setVal(jumpIP);
					}
					else
						throw InvalidDestination(IP.getVal().getAddress().getInt() - 1);
				}
				else
					throw TypeMismatch(IP.getVal().getAddress().getInt() - 1);
			}
			currentIP = IP.getVal().getAddress().getInt();
		}

		if (cmdType == "Return")
		{
			MyType returnAddr(stack.pop());
			// stack.pop();
			IP.setVal(returnAddr);
			currentIP = IP.getVal().getAddress().getInt();
		}
	}

	delete[] memory;
	delete[] cmdArr;
}