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

public final class div_r8 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "div_r8",
          div_r8.class,
          null
          );
      addArgument("op1", R8.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R8 op1 = (R8) getArgument("op1", args);

      return new div_r8(op1);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "div_r8",
          instruction.class,
          null
          );
      addArgument("op1", R8.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final R8 op1 = (R8) getArgument("op1", args);

      return new instruction(
        new div_r8(
          op1
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final R8 op1;

  public div_r8(final R8 op1) {
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
    return String.format("div %s", op1.text(vars__));
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
    vars__.tmpw_op1.access().store(pe__.AX.access().load().divide(Data.zeroExtend(WORD, op1.access(pe__, vars__).load())));
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x2));
    if ((vars__.tmpw_op1.access().load().compareTo(Data.valueOf(WORD, 0xff)) <= 0)) {
      pe__.AH.access().store(Data.coerce(BYTE, pe__.AX.access().load().mod(Data.zeroExtend(WORD, op1.access(pe__, vars__).load()))));
      pe__.AL.access().store(Data.coerce(BYTE, vars__.tmpw_op1.access().load()));
    }
  }
}