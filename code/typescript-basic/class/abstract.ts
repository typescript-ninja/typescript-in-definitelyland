abstract class Animal {
	abstract name: string;
	abstract get poo(): string;

	abstract speak(): string;
	sleep(): string {
		return "zzzZZZ...";
	}
}
// もちろん、abstract classはそのままではインスタンス化できない
// error TS2511: Cannot create an instance of the abstract class 'Animal'.
// new Animal();

class Cat extends Animal {
	// プロパティの実装を強制される
	name = "Cat";
	poo = "poo...";

	// メソッドの実装を強制される
	speak(): string {
		return "meow";
	}
}

new Cat();

export { }
