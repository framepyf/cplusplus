
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <utils/RefBase.h>

using namespace std;
using namespace android;


class Person : public RefBase {

private:
	char *name;
	
	wp<Person> father;
	wp<Person> son;

public:
	Person() {
		cout <<"Pserson()"<<endl;
	}

	Person(char *name) {
		cout <<"Pserson(char *name)"<<endl;
		this->name = name;
	}

	~Person()
	{
		cout << "~Person()"<<endl;
	}

	void setFather(sp<Person> &father)
	{
		this->father = father;
	}

	void setSon(sp<Person> &son)
	{
		this->son = son;
	}

	char *getName(void)
	{
		return name;
	}
	
	void printInfo(void)
	{
		sp<Person> f = father.promote();
		sp<Person> s = son.promote();
		
		//cout<<"just a test function"<<endl;
		cout<<"I am "<<name<<endl;

		if (f != 0)
			cout<<"My Father is "<<f->getName()<<endl;

		if (s != 0)
			cout<<"My Son is "<<s->getName()<<endl;
	}
};

/* 如果对象里含有其他对象成员:
 * 构造时: 先构造其他对象成员, 再构造对象本身
 * 析构时: 顺序刚好相反
 */

void test_func()
{
	/* 1. 对于 new Person()
	 * 1.1 Person对象里的father先被构造
	 * 1.2 Person对象里的son被构造
	 * 1.3 Person对象本身
	 * 2. Person对象的指针传给"sp<Person> father"
	 *    导致: sp(T* other) 被调用
	 *    它增加了这个Person对象的引用计数(现在此值等于1)
	 */
	sp<Person> father = new Person("LiYiShi");


	/* 1. 对于 new Person()
	 * 1.1 Person对象里的father先被构造
	 * 1.2 Person对象里的son被构造
	 * 1.3 Person对象本身
	 * 2. Person对象的指针传给"sp<Person> son"
	 *    导致: sp(T* other) 被调用
	 *    它增加了这个Person对象的引用计数(现在此值等于1)
	 */
	sp<Person> son = new Person("LiErShi");

	/* 它是一个"=" : this->son = son
	 * "="被重载, 它会再次增加该Person对象的引用计数
	 * 所以son对应的Person对象的引用计数增加为2
	 */
	father->setSon(son);

	/* 它是一个"=" : this->father = father
	 * "="被重载, 它会再次增加该Person对象的引用计数
	 * 所以father对应的Person对象的引用计数增加为2
	 */
	son->setFather(father);

	father->printInfo();

	son->printInfo();


}

int main(int argc, char **argv)
{	
	test_func();

	return 0;
}

