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

package ru.ispras.microtesk.model.vmem.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.vmem.PE;
import ru.ispras.microtesk.model.vmem.TempVars;
import ru.ispras.microtesk.model.vmem.mode.*;

import static ru.ispras.microtesk.model.vmem.TypeDefs.*;

public final class st extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "st",
          st.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("addr", REG.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final REG addr = (REG) getArgument("addr", args);

      return new st(rs, addr);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "st",
          instruction.class,
          null
          );
      addArgument("rs", REG.INFO);
      addArgument("addr", REG.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG rs = (REG) getArgument("rs", args);
      final REG addr = (REG) getArgument("addr", args);

      return new instruction(
        new st(
          rs,
          addr
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG rs;
  public final REG addr;

  public st(final REG rs, final REG addr) {
    assert REG.INFO.isSupported(rs);
    assert REG.INFO.isSupported(addr);

    this.rs = rs;
    this.addr = addr;

    addArgument("rs", rs);
    addArgument("addr", addr);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("st %s %s", rs.text(vars__), addr.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("10100000%s%s", rs.image(vars__), addr.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    if (!addr.access(pe__, vars__).bitField(0, 0).load().equals(Data.valueOf(Type.CARD(1), 0x0))) {
      Execution.exception("AddressError");
    }
    pe__.MEM.access(addr.access(pe__, vars__).load().shiftRight(Data.valueOf(HWORD, 0x1))).store(rs.access(pe__, vars__).load());
  }
}