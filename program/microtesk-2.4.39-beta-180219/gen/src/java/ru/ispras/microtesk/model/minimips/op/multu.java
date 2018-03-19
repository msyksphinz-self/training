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

package ru.ispras.microtesk.model.minimips.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.minimips.PE;
import ru.ispras.microtesk.model.minimips.TempVars;
import ru.ispras.microtesk.model.minimips.mode.*;

import static ru.ispras.microtesk.model.minimips.TypeDefs.*;

public final class multu extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "multu",
          multu.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("rt", REG.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final REG rt = (REG) getArgument("rt", args);

      return new multu(rs, rt);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "multu",
          instruction.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("rt", REG.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final REG rt = (REG) getArgument("rt", args);

      return new instruction(
        new multu(
          rs,
          rt
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rs;
  public final REG rt;

  public multu(final REG rs, final REG rt) {
    assert REG.INFO.isSupported(rs);
    assert REG.INFO.isSupported(rt);

    this.rs = rs;
    this.rt = rt;

    addArgument("rs", rs);
    addArgument("rt", rt);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("multu %s, %s", rs.text(vars__), rt.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000000%s%s0000000000011001", rs.image(vars__), rt.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    vars__.mult_result_unsigned.access().store(Data.zeroExtend(Type.CARD(64), rs.access(pe__, vars__).load()).multiply(Data.zeroExtend(Type.CARD(64), rt.access(pe__, vars__).load())));
    pe__.HI.access(0).store(vars__.mult_result_unsigned.access().bitField(63, 32).load());
    pe__.LO.access(0).store(vars__.mult_result_unsigned.access().bitField(31, 0).load());
  }
}