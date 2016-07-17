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
let Animal: AnimalConstructor = class {
	speak() {
		return "???";
	}
};
// Animalはただの変数だが普通に継承できる！
class Cat extends Animal {
	speak() {
		return "meow";
	}
}
let cat: Cat = new Cat();
console.log(cat.speak());

// ECMAScript 2015だと以下のような分すら書けるのでまぁできて然るべきだった
let cat2: Cat = new class extends class {
	speak() {
		return "???";
	}
} {
	speak() {
		return "meow";
	}
}
console.log(cat2.speak());

export { }
