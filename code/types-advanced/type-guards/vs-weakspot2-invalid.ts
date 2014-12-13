class Sample {
	private _tmp: any;
	str: string;
}

// privateなインスタンス変数があるクラスのインスタンスは偽造できない！
// error TS2322: Type '{ _tmp: null; str: string; }' is not
//     assignable to type 'Sample'. Property '_tmp' is private
//     in type 'Sample' but not in type '{ _tmp: null; str: string; }'.
var obj: Sample = {
	_tmp: null,
	str: "Hi!"
};
