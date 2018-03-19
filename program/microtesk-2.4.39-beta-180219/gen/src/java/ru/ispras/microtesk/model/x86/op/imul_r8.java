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

public final class imul_r8 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "imul_r8",
          imul_r8.class,
          null
          );
      addArgument("op1", R8.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R8 op1 = (R8) getArgument("op1", args);

      return new imul_r8(op1);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "imul_r8",
          instruction.class,
          null
          );
      addArgument("op1", R8.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R8 op1 = (R8) getArgument("op1", args);

      return new instruction(
        new imul_r8(
          op1
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R8 op1;

  public imul_r8(final R8 op1) {
    assert R8.INFO.isSupported(op1);

    this.op1 = op1;

    addArgument("op1", op1);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("imul %s", op1.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("1111011011000%s", op1.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    pe__.AX.access().store(Data.signExtend(WORD, Data.cast(SBYTE, pe__.AL.access().load())).multiply(Data.signExtend(WORD, Data.cast(SBYTE, op1.access(pe__, vars__).load()))));
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x2));
    if ((pe__.AH.access().load().equals(Data.valueOf(BYTE, 0x0)) || pe__.AH.access().load().equals(Data.valueOf(BYTE, 0xff)))) {
      pe__.OF.access().store(Data.valueOf(BIT, 0x0));
      pe__.CF.access().store(Data.valueOf(BIT, 0x0));
    } else {
      pe__.OF.access().store(Data.valueOf(BIT, 0x1));
      pe__.CF.access().store(Data.valueOf(BIT, 0x1));
    }
  }
}