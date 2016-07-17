let obj: number | string = null as any;
if (typeof obj === "string") {
	// ここでは string と確定されている！
	obj.charAt(0);
} else {
	// ここでは消去法で number と確定されている！
	obj.toFixed(2);
}

export {}
