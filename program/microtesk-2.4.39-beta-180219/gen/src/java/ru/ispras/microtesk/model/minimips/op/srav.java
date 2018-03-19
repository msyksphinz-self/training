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

public final class srav extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "srav",
          srav.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("rt", REG.INFO);
      addArgument("rs", REG.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final REG rt = (REG) getArgument("rt", args);
      final REG rs = (REG) getArgument("rs", args);

      return new srav(rd, rt, rs);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "srav",
          instruction.class,
          null
          );
      addArgument("rd", REG.INFO);
      addArgument("rt", REG.INFO);
      addArgument("rs", REG.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rd = (REG) getArgument("rd", args);
      final REG rt = (REG) getArgument("rt", args);
      final REG rs = (REG) getArgument("rs", args);

      return new instruction(
        new srav(
          rd,
          rt,
          rs
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rd;
  public final REG rt;
  public final REG rs;

  public srav(final REG rd, final REG rt, final REG rs) {
    assert REG.INFO.isSupported(rd);
    assert REG.INFO.isSupported(rt);
    assert REG.INFO.isSupported(rs);

    this.rd = rd;
    this.rt = rt;
    this.rs = rs;

    addArgument("rd", rd);
    addArgument("rt", rt);
    addArgument("rs", rs);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("srav %s, %s, %s", rd.text(vars__), rt.text(vars__), rs.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("00000000000%s%s%s000111", rt.image(vars__), rd.image(vars__), rs.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    rd.access(pe__, vars__).store(rt.access(pe__, vars__).load().shiftRight(rs.access(pe__, vars__).bitField(4, 0).load()));
  }
}