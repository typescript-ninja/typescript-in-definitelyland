declare module jquery {
	interface Element {
		addClass(className: string): Element;
		html(htmlString: string): Element;
		val(): any;
		empty(): Element;
		append(content1: Element, ...content2: any[]): Element;
		appendTo(target: Element): Element;
	}

	interface Static {
		ajax(settings: AjaxSettings): any;
		(selector: string, context?: Element): Element;
		(element: Element): Element;
	}

	interface AjaxSettings {
		data?: any;
		type?: string;
		url?: string;
	}

	interface Promise<T> {
		state(): string;
		then<U>(fullfill: (value: T) => U, reject?: (...reasons: any[]) => U): Promise<U>;
	}

	interface Deferred<T> extends Promise<T> {
		reject(...args: any[]): Deferred<T>;
		resolve(value?: T, ...args: any[]): Deferred<T>;
	}
}

declare var $: jquery.Static;
