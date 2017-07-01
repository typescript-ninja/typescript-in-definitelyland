class Base {
	// インスタンス変数
	num = 1;

	// 初期値を与えない場合は型の指定が必要
	str: string;

	// プロパティ名に ? をつけると省略可能（undefinedである可能性がある）ことを表せる
	regExpOptional?: RegExp;

	constructor(str: string) {
		// str は省略可能じゃないのでコンストラクタで初期値を設定しなければならない
		// 設定し忘れても現在のTypeScriptはエラーにしてくれないので注意が必要…
		this.str = str;
	}

	// メソッドの定義 返り値は省略してもOK
	hello(): string {
		return `Hello, ${this.str}`;
	}

	get regExp() {
		if (!this.regExpOptional) {
			return new RegExp("test");
		}

		return this.regExpOptional;
	}
}

const base = new Base("world");
console.log(base.hello());

export { };
