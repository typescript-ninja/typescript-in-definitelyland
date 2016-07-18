class Base {
	// インスタンス変数
	numA: number;
	strA = "string";
	public numB: number;
	private numC: number;
	protected numD: number;

	// クラス変数
	static numA: number;
	public static numB: number;
	private static numC: number;
	protected static numD: number;

	// コンストラクタ
	constructor(boolA: boolean,
		public boolB: boolean,
		private boolC: boolean,
		protected boolD: boolean) {
		// エラー消し 一回も使われない可能性があると怒られる
		console.log(boolA, this.numC, this.boolC, Base.numC);
	}

	// メソッド
	hello(word: string): string {
		return "Hello, " + word;
	}

	// get, setアクセサ
	// コンパイル時に --target es5 以上が必要
	/** @internal **/
	private _date: Date;
	get dateA(): Date {
		return this._date;
	}
	set dateA(value: Date) {
		this._date = value;
	}
}

let obj = new Base(true, false, true, false);
obj.numA;
obj.strA;
obj.numB;
// obj.numC; // private   なメンバにはアクセスできない
// obj.numD; // protected なメンバにもアクセスできない
obj.boolB;
// obj.boolC; // private   なメンバにはアクセスできない
// obj.boolD; // protected なメンバにもアクセスできない
obj.hello("TypeScript");
obj.dateA = new Date();
obj.dateA;

export { }
