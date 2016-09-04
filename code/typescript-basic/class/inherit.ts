class Base {
	greeting(name: string) {
		return "Hi! " + name;
	}
}

class Inherit extends Base {
	greeting(name: string) {
		return super.greeting(name) + ". How are you?";
	}
}

let obj = new Inherit();
// Hi! TypeScript. How are you? と出力される
console.log(obj.greeting("TypeScript"));

export { }
