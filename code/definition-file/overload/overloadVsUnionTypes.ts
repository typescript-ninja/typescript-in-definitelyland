// union types未使用
declare function hello(word: string): string;
declare function hello(callback: () => string): string;

hello("TypeScript");
hello(() => "function");

// union typesあり
declare function bye(word: string | { (): string; }): string;

bye("JavaScript");
bye(() => "function");
