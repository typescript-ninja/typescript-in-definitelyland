namespace a {
	// export してないものは外部からは見えない
	class Sample {
		hello(word = "TypeScript") {
			return `Hello, ${word}`;
		}
	}

	export interface Hello {
		hello(word?: string): string;
	}
	export let obj: Hello = new Sample();
}
namespace a {
	export function bye(word = "JavaScript") {
		return `Bye, ${word}`;
	}

	// 定義を分けてしまうと同名のモジュールでもexportされていないものは見えない
	// error TS2304: Cannot find name 'Sample'.
	// let tmp = new Sample();
}

// ネストしたnamespace
namespace b {
	export namespace c {
		export function hello() {
			return a.obj.hello();
		}
	}
}
// ネストしたnamespaceの短縮表記も存在する
namespace d.e {
	export function hello() {
		return a.obj.hello();
	}
}

// Hello, TypeScript と表示される
console.log(b.c.hello());
// Hello, TypeScript と表示される
console.log(d.e.hello());
