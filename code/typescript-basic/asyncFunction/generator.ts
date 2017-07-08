function* generagtorHello(): IterableIterator<string> {
    while (true) {
        yield "hello";
    }
}

async function* asyncGeneragtorHello(): AsyncIterableIterator<string> {
    while (true) {
        await sleep(100);
        yield "hello";
    }
}

function sleep(millisecond: number) {
    return new Promise(resolve => {
        setTimeout(() => {
            resolve();
        }, millisecond);
    });
}
