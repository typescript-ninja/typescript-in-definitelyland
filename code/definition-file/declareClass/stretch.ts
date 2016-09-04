// classはopen-endedになったため同名のinterfaceで拡張可能に
class Person {
	name: string;
}
interface Person {
	age: number;
}
let p: Person = new Person();
// 両方アクセス可能！
console.log(p.name, p.age);

// interfaceを使ったクラスの構成でも
interface AnimalConstructor {
	new (): Animal;
}
interface Animal {
	speak(): string;
}
/* tslint:disable:variable-name */
let Animal: AnimalConstructor = class {
	speak() {
		return "???";
	}
};
/* tslint:enable:variable-name */
// Animalはただの変数だが普通に継承できる！
class Cat extends Animal {
	speak() {
		return "meow";
	}
}
let cat: Cat = new Cat();
console.log(cat.speak());

export { }
