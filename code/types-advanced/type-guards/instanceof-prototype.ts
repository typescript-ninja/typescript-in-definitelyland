interface AStatic {
	new (): AInstance;
	prototype: AInstance;
}
interface AInstance {
	str: string;
}
declare var A: AStatic;

var obj: AInstance | {};
if (obj instanceof A) {
	// ここではAInstanceと確定されている！
	obj.str;
}
