// オプショナルな引数の後に省略不可な引数がきてはいけない
// error TS1016: A required parameter cannot follow an optional parameter.
function funcA(arg1?: string, arg2: string) {
	return `Hello, ${arg1}, ${arg2}`;
}

// 可変長引数は必ず最後じゃないといけない
// error TS1014: A rest parameter must be last in a parameter list.
function funcB(...args: string[], rest: string) {
	return `Hello, ${args.join(", ")} and ${rest}`;
}

export { }
