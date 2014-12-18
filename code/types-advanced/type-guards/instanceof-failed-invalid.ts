interface AStatic {
	new (): AInstance;
	// prototype: AInstance; がない！
}
interface AInstance {
	str: string;
}
declare var A: AStatic;

var obj: AInstance | Date;
if (obj instanceof A) {
	// ここではAInstanceに確定されていてほしかった…
	// error TS2339: Property 'str' does not exist on type 'Date | AInstance'.
	obj.str;
}
