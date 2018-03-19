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

public final class jr extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "jr",
          jr.class,
          null
          );
      addArgument("addr", REG.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG addr = (REG) getArgument("addr", args);

      return new jr(addr);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "jr",
          instruction.class,
          null
          );
      addArgument("addr", REG.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final REG addr = (REG) getArgument("addr", args);

      return new instruction(
        new jr(
          addr
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final REG addr;

  public jr(final REG addr) {
    assert REG.INFO.isSupported(addr);

    this.addr = addr;

    addArgument("addr", addr);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("jr %s", addr.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000000%s000000000000000001000", addr.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    pe__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
    pe__.JMPADDR.access().store(addr.access(pe__, vars__).load());
  }
}