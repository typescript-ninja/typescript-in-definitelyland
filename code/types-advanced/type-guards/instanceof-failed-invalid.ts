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
	// ここでは AInstance に確定されていてほしかった…
	// error TS2339: Property 'str' does not exist on type 'Date | AInstance'.
	obj.str;
}
