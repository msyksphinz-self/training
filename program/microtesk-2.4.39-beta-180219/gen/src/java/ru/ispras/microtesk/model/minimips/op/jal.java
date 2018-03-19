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

public final class jal extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "jal",
          jal.class,
          null
          );
      addArgument("addr", JUMP_ADDR.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive addr = (IsaPrimitive) getArgument("addr", args);

      return new jal(addr);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "jal",
          instruction.class,
          null
          );
      addArgument("addr", JUMP_ADDR.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final IsaPrimitive addr = (IsaPrimitive) getArgument("addr", args);

      return new instruction(
        new jal(
          addr
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final IsaPrimitive addr;

  public jal(final IsaPrimitive addr) {
    assert JUMP_ADDR.INFO.isSupported(addr);

    this.addr = addr;

    addArgument("addr", addr);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("jal %s", addr.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("000011%s", addr.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    pe__.GPR.access(31).store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x4)).add(Data.valueOf(WORD, 0x4)));
    vars__.nia.access().store(pe__.CIA.access().load().add(Data.valueOf(WORD, 0x4)));
    vars__.temp_jump.access().store(Data.signExtend(Type.CARD(28), addr.access(pe__, vars__).load()).shiftLeft(Data.valueOf(Type.CARD(28), 0x2)));
    pe__.BRANCH.access().store(Data.valueOf(BIT, 0x1));
    pe__.JMPADDR.access().store(Location.concat(vars__.nia.access().bitField(31, 28), vars__.temp_jump.access()));
  }
}