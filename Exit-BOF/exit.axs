var metadata = {
    name: "Exit-BOF",
    description: "Terminate the beacon process or thread",
};

var cmd_exit_process = ax.create_command("process", "Terminate the current beacon process via NTDLL!RtlExitUserProcess(0). [NOISE: terminal]", "exit process");
cmd_exit_process.setPreHook(function (id, cmdline, parsed_json, ...parsed_lines) {
    var bof_path = ax.script_dir() + "_bin/exitprocess." + ax.arch(id) + ".o";
    ax.execute_alias(id, cmdline, `execute bof "${bof_path}"`, "Task: Exit process (BOF)", null);
});

var cmd_exit_thread = ax.create_command("thread", "Terminate the current beacon thread via NTDLL!RtlExitUserThread(0). [NOISE: terminal]", "exit thread");
cmd_exit_thread.setPreHook(function (id, cmdline, parsed_json, ...parsed_lines) {
    var bof_path = ax.script_dir() + "_bin/exitthread." + ax.arch(id) + ".o";
    ax.execute_alias(id, cmdline, `execute bof "${bof_path}"`, "Task: Exit thread (BOF)", null);
});

var cmd_exit = ax.create_command("exit", "Terminate the current beacon process or thread");
cmd_exit.addSubCommands([cmd_exit_process, cmd_exit_thread]);

var group_exit = ax.create_commands_group("Exit-BOF", [cmd_exit]);
ax.register_commands_group(group_exit, ["beacon", "gopher", "kharon"], ["windows"], []);
