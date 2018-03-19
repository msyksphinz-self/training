/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;
import ru.ispras.microtesk.model.x86.mode.*;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class test_r16r16 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "test_r16r16",
          test_r16r16.class,
          null
          );
      addArgument("op1", R16.INFO);
      addArgument("op2", R16.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R16 op1 = (R16) getArgument("op1", args);
      final R16 op2 = (R16) getArgument("op2", args);

      return new test_r16r16(op1, op2);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "test_r16r16",
          instruction.class,
          null
          );
      addArgument("op1", R16.INFO);
      addArgument("op2", R16.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R16 op1 = (R16) getArgument("op1", args);
      final R16 op2 = (R16) getArgument("op2", args);

      return new instruction(
        new test_r16r16(
          op1,
          op2
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R16 op1;
  public final R16 op2;

  public test_r16r16(final R16 op1, final R16 op2) {
    assert R16.INFO.isSupported(op1);
    assert R16.INFO.isSupported(op2);

    this.op1 = op1;
    this.op2 = op2;

    addArgument("op1", op1);
    addArgument("op2", op2);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("test %s, %s", op1.text(vars__), op2.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("1000010111%s%s", op1.image(vars__), op2.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    new test_proc_word(new Immediate(op1.access(pe__, vars__).load().add(Data.valueOf(WORD, 0x0))), new Immediate(op2.access(pe__, vars__).load().add(Data.valueOf(WORD, 0x0)))).execute(pe__, vars__);
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x2));
  }
}