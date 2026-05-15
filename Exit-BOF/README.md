# Exit-BOF

Terminate the beacon process or thread

## exit process

Terminate the current beacon process via `NTDLL!RtlExitUserProcess(0)`. [NOISE: terminal]

```
exit process
```

## exit thread

Terminate the current beacon thread via `NTDLL!RtlExitUserThread(0)`. [NOISE: terminal]

```
exit thread
```
