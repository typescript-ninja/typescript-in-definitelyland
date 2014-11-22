interface JQuery {
    addClass(className: string): JQuery;
    html(htmlString: string): JQuery;
    val(): any;
    empty(): JQuery;
    append(content1: JQuery, ...content2: any[]): JQuery;
    appendTo(target: JQuery): JQuery;
}

interface JQueryStatic {
    ajax(settings: JQueryAjaxSettings): any;
    (selector: string, context?: Element): JQuery;
    (element: Element): JQuery;
}

interface JQueryAjaxSettings {
    data?: any;
    type?: string;
    url?: string;
}

interface JQueryPromise<T> {
    state(): string;
    then<U>(doneFilter: (value: T) => U, failFilter?: (...reasons: any[]) => U, progressFilter?: (...progression: any[]) => any): JQueryPromise<U>;
}

interface JQueryDeferred<T> extends JQueryPromise<T> {
    reject(...args: any[]): JQueryDeferred<T>;
    resolve(value?: T, ...args: any[]): JQueryDeferred<T>;
}

declare var $: JQueryStatic;
