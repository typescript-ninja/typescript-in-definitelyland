// lib.dom.d.ts から抜粋
// 第一引数で指定するイベントによってリスナーで得られるイベントの型が違う
interface HTMLBodyElement extends HTMLElement {
	addEventListener(type: "change", listener: (this: this, ev: Event) => any, useCapture?: boolean): void;
	addEventListener(type: "click", listener: (this: this, ev: MouseEvent) => any, useCapture?: boolean): void;
	addEventListener(type: "keydown", listener: (this: this, ev: KeyboardEvent) => any, useCapture?: boolean): void;
	addEventListener(type: "keypress", listener: (this: this, ev: KeyboardEvent) => any, useCapture?: boolean): void;
	addEventListener(type: "keyup", listener: (this: this, ev: KeyboardEvent) => any, useCapture?: boolean): void;
	addEventListener(type: "wheel", listener: (this: this, ev: WheelEvent) => any, useCapture?: boolean): void;
	addEventListener(type: string, listener: EventListenerOrEventListenerObject, useCapture?: boolean): void;
}
