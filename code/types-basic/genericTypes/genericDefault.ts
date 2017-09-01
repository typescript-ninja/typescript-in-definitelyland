class DataContainer<T = string> {
	data?: T

	print() {
		console.log(this.data);
	}
}

// 省略したらTはstring
const obj1 = new DataContainer();
obj1.data = "";
// defaultはstringなのでこれはダメ
// obj1.data = 1;

// 省略しなかったのでTはnumber
const obj2 = new DataContainer<number>();
obj2.data = 1;
