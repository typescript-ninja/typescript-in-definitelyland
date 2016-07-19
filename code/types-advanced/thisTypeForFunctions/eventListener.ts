// lib.dom.d.ts から抜粋
// listenerの仮引数の先頭が偽の仮引数で、thisの型の指定が行われている
interface HTMLBodyElement extends HTMLElement {
	addEventListener(type: "click", listener: (this: this, ev: MouseEvent) => any, useCapture?: boolean): void;
	addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}

let el1: HTMLBodyElement = null as any;
el1.addEventListener("click", function() {
	// thisの型はHTMLBodyElement
	this.innerText = "Hi!";
});
el1.addEventListener("click", () => {
	// アロー関数の場合thisの値は変えられない
	// error TS2683: 'this' implicitly has type 'any'
	//   because it does not have a type annotation.
	// this.innerText = "Hi!";
});

let el2: HTMLDivElement = null as any;
el2.addEventListener("click", function() {
	// thisの型はHTMLDivElement
	this.innerText = "Hi!";
});

export { }
