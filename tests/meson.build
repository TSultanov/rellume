casefiles = {
  'x86_64': [
    'cases_basic.txt',
    'cases_modrm.txt',
    'cases_string.txt',
    'cases_sse.txt',
  ],
  'rv64': [
    'cases_rv64_basic.txt',
  ],
  'aarch64': [
    'a64_data_proc.txt',
    'a64_loads_stores.txt',
    'a64_branches_and_cond.txt',
    'a64_scalar_fp.txt',
    'a64_simd.txt',
  ]
}
llvm_targets = {
  'x86_64': 'X86',
  'rv64': 'RISCV',
  'aarch64': 'AArch64',
}

test_architectures = []
test_assembler_flags = []
cpp = meson.get_compiler('cpp')
foreach arch : architectures
  if not llvm_targets.has_key(arch)
    continue
  endif
  tgt_init = 'LLVMInitialize@0@Target'.format(llvm_targets[arch])
  test_code = '#include<llvm/Support/TargetSelect.h>\nint main(){@0@();}'.format(tgt_init)
  if cpp.compiles(test_code, dependencies: libllvm)
    test_architectures += arch
    test_assembler_flags += '-DTARGET_@0@'.format(arch.to_upper())
  else
    warning('provided LLVM does not support assembling for @0@'.format(arch))
  endif
endforeach

assembler = executable('test_assembler', 'test_assembler.cc',
                       cpp_args: test_assembler_flags,
                       dependencies: [libllvm])
driver = executable('test_driver', 'test_driver.cc', 'minilibc.cc', cpustruct_priv, dependencies: [librellume], pie: true)

python3 = find_program('python3')
foreach arch : test_architectures
  parsed_cases = custom_target('parsed_cases_@0@.txt'.format(arch),
                               command: [python3, files('test_parser.py'), '-o', '@OUTPUT@', '-a', assembler, '-A', arch, '@INPUT@'],
                               input: files(casefiles[arch]),
                               output: 'parsed_cases_@0@.txt'.format(arch))
  test('emulation-@0@'.format(arch), driver,
       args: ['-A', arch, parsed_cases], protocol: 'tap')
  test('emulation-@0@-pic'.format(arch), driver,
       args: ['-A', arch, '-p', parsed_cases], protocol: 'tap')
  test('emulation-@0@-jit'.format(arch), driver,
       args: ['-A', arch, '-j', parsed_cases], protocol: 'tap', timeout: 120)
endforeach
