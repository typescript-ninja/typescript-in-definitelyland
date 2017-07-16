const obj1 = {
	name: "maya",
	greeting() {
		console.log(`Hello, ${this.name}`);

		// 存在しないプロパティにアクセスするとちゃんとエラーにしてくれる
		// error TS2339: Property 'notExists' does not exist on 
		//   type '{ name: string; greeting(): void; }'.
		// console.log(`Hello, ${this.notExists}`);
	},
};
console.log(obj1.greeting());

// ThisType を使ってthisの値を無理やり制御する
interface A {
	name: string;
}
interface B {
	hello(): void;
}

// objの型はBであり、obj内でのthisの型はAと明示的に指定する
const obj: B & ThisType<A> = {
	hello() {
		// this.name は A的には存在する！
		console.log(`Hello, ${this.name}`);
		// Aに存在しないものは存在しない扱い ちなみに this.hello も存在しない扱い
		// error TS2339: Property 'notExists' does not exist on type 'A'.
		// console.log(`Hello, ${this.notExists}`);
	},
};
// objは B なので普通にhelloにアクセスできる
obj.hello();

export { }
