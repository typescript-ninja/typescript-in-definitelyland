function test<T>(...args: T[]): T[] {
	return args;
}
// (number | boolean)[] にはならなかった。残念。
// error TS2453: The type argument for type parameter 'T' cannot be inferred from
// the usage. Consider specifying the type arguments explicitly.
var v = test(1, true);
