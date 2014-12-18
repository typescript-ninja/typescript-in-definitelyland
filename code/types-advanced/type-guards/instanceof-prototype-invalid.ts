interface AStatic {
	// 次のどちらかがないとFunctionとの互換性が無い
	// (): AInstance;
	// new (): AInstance;
	prototype: AInstance;
}
interface AInstance {
	str: string;
}
declare var A: AStatic;

var obj: any;
// error TS2359: The right-hand side of an 'instanceof'
// expression must be of type 'any' or of a type assignable
// to the 'Function' interface type.
if (obj instanceof A) {
}
