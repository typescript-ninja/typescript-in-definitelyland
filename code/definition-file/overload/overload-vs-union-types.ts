// union types 以前
declare function hello(word: string): string;
declare function hello(callback: () => string): string;

hello("TypeScript");
hello(() => "function");

// union types 以後 好みの問題かもね
declare function bye(word: string | { (): string; }): string;

bye("JavaScript");
bye(() => "function");
