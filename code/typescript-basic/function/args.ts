function hello(...args: string[]) {
	return `Hello, ${args.join(" & ")}`;
}
// Hello, TS & JS と表示される
console.log(hello("TS", "JS"));

export { }
