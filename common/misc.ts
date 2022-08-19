import * as child_process from 'child_process';
import { ExecOptions } from 'child_process';

type ExecuteOptions = ExecOptions & {
    stdout?: NodeJS.WriteStream;
    stderr?: NodeJS.WriteStream;
};

export function execute(command: string, { stdout, stderr, ...options }: ExecuteOptions = {}): Promise<string> {
    return new Promise((resolve, reject) => {
        const child = child_process.exec(command, options, (error, stdout, stderr) => {
            if (error) return reject(error.code ? new Error(stderr) : error);
            resolve(stdout);
        });
        if (child.stdout && stdout) child.stdout.pipe(stdout);
        if (child.stderr && stderr) child.stderr.pipe(stderr);
    });
}